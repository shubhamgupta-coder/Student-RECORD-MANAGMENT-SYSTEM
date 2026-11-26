"""PIU Waste Classifier: complete source code in one Python file.

This educational Windows desktop app can classify waste photographs, accept a
student's correction, build a local feedback dataset and fine-tune the final model
layer.  The code is intentionally commented in simple English for Polymer Science
students who are learning how data can improve an AI model.

IMPORTANT: A Python source file cannot sensibly contain the large trained neural
network.  Keep the Models folder beside the Source_Code folder as described in the
VS Code guide.  All application logic is contained in this one .py file.
"""

from __future__ import annotations

import hashlib
import json
import os

# Configure TensorFlow logging BEFORE tensorflow is imported anywhere.
os.environ.setdefault("TF_CPP_MIN_LOG_LEVEL", "2")

import shutil
import subprocess
import sys
import threading
from datetime import datetime, timezone
from pathlib import Path
from tkinter import BOTH, DISABLED, END, LEFT, RIGHT, X, filedialog, messagebox, ttk
import tkinter as tk
from typing import TYPE_CHECKING, Callable

import numpy as np
from PIL import Image, ImageEnhance, ImageOps, ImageTk, UnidentifiedImageError

if TYPE_CHECKING:
    import tensorflow as tf


# A Python dictionary stores information as key/value pairs.  Here, the key is the
# material name and the value is another dictionary containing a short explanation
# and practical separation advice.
MATERIAL_INFORMATION = {
    "Batteries": {
        "display_name": "Batteries",
        "description": (
            "Batteries store chemical energy and may contain metals or electrolytes "
            "that should not enter ordinary mixed waste."
        ),
        "advice": "Keep terminals protected and use an authorised battery collection point.",
    },
    "Clothes": {
        "display_name": "Clothes / Textiles",
        "description": (
            "Textiles may contain natural fibres, synthetic polymer fibres, dyes, "
            "buttons, zips and blended materials."
        ),
        "advice": "Reuse or donate wearable clothes; send damaged textiles to a textile recycler.",
    },
    "E-waste": {
        "display_name": "Electronic Waste",
        "description": (
            "Electronic waste combines polymers, metals, glass and electronic components. "
            "It needs controlled dismantling and material recovery."
        ),
        "advice": "Use an authorised e-waste collection centre and never burn electronic parts.",
    },
    "Glass": {
        "display_name": "Glass",
        "description": (
            "Container glass can often be cleaned, sorted by colour, crushed into cullet "
            "and remelted to manufacture new glass products."
        ),
        "advice": "Remove lids, rinse containers and follow local colour-sorting rules.",
    },
    # The original saved model used the spelling “Blubs”.  We keep that key so the
    # output order remains compatible, but show the correct wording to users.
    "Light Blubs": {
        "display_name": "Light Bulbs",
        "description": (
            "Different lamp technologies contain different materials.  Fluorescent lamps "
            "may contain mercury, while LED lamps contain electronic components."
        ),
        "advice": "Keep bulbs unbroken and take them to a lamp or hazardous-waste collection point.",
    },
    "Metal": {
        "display_name": "Metal",
        "description": (
            "Metals such as steel and aluminium can be separated, melted and returned to "
            "manufacturing many times."
        ),
        "advice": "Empty and rinse containers; separate metal parts from attached plastics when practical.",
    },
    "Organic": {
        "display_name": "Organic Waste",
        "description": (
            "Food and plant residues are biodegradable.  Controlled composting or anaerobic "
            "digestion can recover nutrients or energy."
        ),
        "advice": "Keep organic waste free from plastic packaging and other contaminants.",
    },
    "Paper": {
        "display_name": "Paper",
        "description": (
            "Paper consists mainly of cellulose fibres.  Clean, dry fibres can be repulped, "
            "although fibre quality decreases after repeated recycling."
        ),
        "advice": "Keep paper dry and remove food, plastic films and other non-paper materials.",
    },
    "Plastic": {
        "display_name": "Plastic",
        "description": (
            "Plastic products are made from different polymers.  PET, HDPE, PVC, LDPE, PP, "
            "PS and multilayer materials require different recycling routes."
        ),
        "advice": (
            "Check the resin identification code, empty and rinse the item, and follow local "
            "collection rules.  A visual prediction does not determine the exact polymer grade."
        ),
    },
}


# Output position 0 means Batteries, output position 1 means Clothes, and so on.
# This historical order must never change unless the neural network is retrained.
CLASS_NAMES = [
    "Batteries",
    "Clothes",
    "E-waste",
    "Glass",
    "Light Blubs",  # Historical model spelling; the interface displays Light Bulbs.
    "Metal",
    "Organic",
    "Paper",
    "Plastic",
]


class WasteClassifier:
    """Load a factory/student model and classify one photograph at a time."""

    def __init__(
        self,
        factory_model_path: Path,
        student_model_path: Path | None = None,
        correction_lookup: Callable[[Path], dict | None] | None = None,
    ) -> None:
        self.factory_model_path = Path(factory_model_path)
        self.student_model_path = Path(student_model_path) if student_model_path else None
        self.correction_lookup = correction_lookup
        self.model: tf.keras.Model | None = None
        self.loaded_model_path: Path | None = None
        self._lock = threading.RLock()
        self.reload_model()

    def preferred_model_path(self) -> Path:
        """Use a student's trained model when available; otherwise use the factory model."""

        if self.student_model_path and self.student_model_path.exists():
            return self.student_model_path
        return self.factory_model_path

    def reload_model(self) -> None:
        """Reload model weights after a learning run or factory reset."""

        import tensorflow as tf  # Delayed import keeps UI startup responsive.

        with self._lock:
            model_path = self.preferred_model_path()
            if not model_path.exists():
                raise FileNotFoundError(
                    f"Model file was not found:\n{model_path}\n\n"
                    "Place 'classifyWaste.h5' inside the Models folder beside Source_Code."
                )

            # compile=False avoids loading training settings that are unnecessary for
            # prediction. It also makes older educational H5 models more compatible.
            model = tf.keras.models.load_model(model_path, compile=False)
            number_of_outputs = int(model.output_shape[-1])
            if number_of_outputs != len(CLASS_NAMES):
                raise ValueError(
                    f"The model has {number_of_outputs} outputs, but the app expects "
                    f"{len(CLASS_NAMES)} classes."
                )
            self.model = model
            self.loaded_model_path = model_path

    @staticmethod
    def prepare_image(image_path: Path) -> np.ndarray:
        """Open one image and convert it to the numerical format used in training."""

        # Apply EXIF orientation so phone photos match the preview and augmentation,
        # resize to 224 x 224 RGB, and normalise pixel values to 0-1.
        with Image.open(image_path) as opened_image:
            image = (
                ImageOps.exif_transpose(opened_image)
                .convert("RGB")
                .resize((224, 224), Image.Resampling.BILINEAR)
            )
            image_array = np.asarray(image, dtype=np.float32) / 255.0

        # A network accepts batches. This adds a batch dimension to our one image:
        # (224, 224, 3) becomes (1, 224, 224, 3).
        return np.expand_dims(image_array, axis=0)

    def predict(self, image_path: Path) -> dict:
        """Return the model result plus any active exact-image teaching correction."""

        with self._lock:
            if self.model is None:
                raise RuntimeError("The model is not loaded.")
            prepared = self.prepare_image(image_path)
            probabilities = self.model.predict(prepared, verbose=0)[0]

        raw_index = int(np.argmax(probabilities))
        raw_class = CLASS_NAMES[raw_index]

        # A correction becomes active only after the student runs Improve AI. This
        # exact-image memory guarantees the taught file is recalled correctly, while
        # the fine-tuned weights are responsible for generalising to similar images.
        correction = self.correction_lookup(Path(image_path)) if self.correction_lookup else None
        corrected_class = correction.get("correct_class") if correction else None
        learned_from_feedback = corrected_class in CLASS_NAMES
        if learned_from_feedback and corrected_class is not None:
            predicted_class = corrected_class
            predicted_index = CLASS_NAMES.index(predicted_class)
        else:
            predicted_class = raw_class
            predicted_index = raw_index

        information = MATERIAL_INFORMATION[predicted_class]
        return {
            "class_name": predicted_class,
            "display_name": information["display_name"],
            "confidence": float(probabilities[predicted_index]) * 100.0,
            "description": information["description"],
            "advice": information["advice"],
            "raw_class_name": raw_class,
            "raw_display_name": MATERIAL_INFORMATION[raw_class]["display_name"],
            "learned_from_feedback": learned_from_feedback,
            "all_probabilities": {
                CLASS_NAMES[index]: float(value) * 100.0
                for index, value in enumerate(probabilities)
            },
        }


IMAGE_EXTENSIONS = {".jpg", ".jpeg", ".png"}


def default_student_data_directory() -> Path:
    """Return a writable per-student folder on Windows."""

    # LOCALAPPDATA normally looks like C:\Users\Name\AppData\Local.  The fallback
    # also keeps the program usable in unusual classroom Windows configurations.
    local_app_data = os.environ.get("LOCALAPPDATA")
    base = Path(local_app_data) if local_app_data else Path.home() / "AppData" / "Local"
    return base / "PIUPlasticWasteClassifier"


def file_sha256(file_path: Path) -> str:
    """Create a stable fingerprint for exact-image correction memory."""

    digest = hashlib.sha256()
    with Path(file_path).open("rb") as file_handle:
        for block in iter(lambda: file_handle.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def _atomic_write_text(target_path: Path, text: str) -> None:
    """Write text atomically using a temporary file and os.replace."""

    target_path.parent.mkdir(parents=True, exist_ok=True)
    temporary_path = target_path.with_suffix(target_path.suffix + ".tmp")
    temporary_path.write_text(text, encoding="utf-8")
    os.replace(temporary_path, target_path)


class StudentLearningManager:
    """Manage corrected images, learning records and personalised model files."""

    def __init__(self, class_names: list[str], data_root: Path | None = None) -> None:
        self.class_names = list(class_names)
        self.data_root = Path(data_root) if data_root else default_student_data_directory()
        self.feedback_directory = self.data_root / "Feedback_Dataset"
        self.models_directory = self.data_root / "Models"
        self.records_path = self.data_root / "feedback_records.json"
        self.training_report_path = self.data_root / "latest_training_report.json"
        self.student_model_path = self.models_directory / "student_model.keras"
        self.backup_model_path = self.models_directory / "student_model_backup.keras"

        # Create all nine class folders immediately.  Students can therefore inspect
        # the learning dataset in File Explorer even before adding a correction.
        self.models_directory.mkdir(parents=True, exist_ok=True)
        for class_name in self.class_names:
            self.class_directory(class_name).mkdir(parents=True, exist_ok=True)
        if not self.records_path.exists():
            self._save_records([])

    @staticmethod
    def _folder_name(class_name: str) -> str:
        """Convert a display/model name into a simple Windows folder name."""

        # Correct the historical model spelling only in the user-visible folder.
        if class_name == "Light Blubs":
            return "light_bulbs"
        return class_name.lower().replace("-", "_").replace(" ", "_")

    def class_directory(self, class_name: str) -> Path:
        """Return the feedback folder belonging to one model class."""

        if class_name not in self.class_names:
            raise ValueError(f"Unknown class: {class_name}")
        return self.feedback_directory / self._folder_name(class_name)

    def _load_records(self) -> list[dict]:
        """Read the small JSON learning log; recover safely if it is damaged."""

        try:
            value = json.loads(self.records_path.read_text(encoding="utf-8"))
            return value if isinstance(value, list) else []
        except (OSError, json.JSONDecodeError):
            return []

    def _save_records(self, records: list[dict]) -> None:
        """Write records atomically so a power interruption cannot leave half a file."""

        _atomic_write_text(self.records_path, json.dumps(records, indent=2))

    @staticmethod
    def _save_clean_jpeg(source_path: Path, destination: Path) -> None:
        """Re-encode an image as an EXIF-corrected RGB JPEG."""

        destination.parent.mkdir(parents=True, exist_ok=True)
        with Image.open(source_path) as opened_image:
            clean_image = ImageOps.exif_transpose(opened_image).convert("RGB")
            clean_image.save(destination, "JPEG", quality=94, optimize=True)

    def add_correction(
        self,
        image_path: Path,
        correct_class: str,
        predicted_class: str,
        model_score: float,
    ) -> dict:
        """Copy one corrected image into its true class and log the teaching event."""

        source_path = Path(image_path)
        if correct_class not in self.class_names:
            raise ValueError(f"Unknown correction class: {correct_class}")
        if source_path.suffix.lower() not in IMAGE_EXTENSIONS:
            raise ValueError("Only JPEG and PNG photographs can be added for learning.")

        image_hash = file_sha256(source_path)
        destination = self.class_directory(correct_class) / f"student_{image_hash[:16]}.jpg"
        records = self._load_records()

        # If the same file was already taught, update its label AND ensure the stored
        # JPEG lives in the newly chosen class directory instead of the old one.
        existing = next(
            (
                record
                for record in records
                if image_hash in (record.get("sha256"), record.get("stored_sha256"))
            ),
            None,
        )
        if existing:
            old_path_value = existing.get("stored_path", "")
            old_path = Path(old_path_value) if old_path_value else None
            if old_path and old_path.exists():
                if old_path.resolve() != destination.resolve():
                    destination.parent.mkdir(parents=True, exist_ok=True)
                    shutil.move(str(old_path), str(destination))
            else:
                self._save_clean_jpeg(source_path, destination)

            stored_hash = file_sha256(destination)
            existing.update(
                {
                    "stored_path": str(destination),
                    "stored_sha256": stored_hash,
                    "correct_class": correct_class,
                    "predicted_class_before": predicted_class,
                    "model_score_before": float(model_score),
                    "active": False,
                    "updated_at_utc": datetime.now(timezone.utc).isoformat(),
                }
            )
            self._save_records(records)
            return existing

        # Re-encode as RGB JPEG.  This applies phone-camera orientation metadata and
        # avoids depending on the original file after the student moves/deletes it.
        self._save_clean_jpeg(source_path, destination)
        stored_hash = file_sha256(destination)

        record = {
            "sha256": image_hash,
            "stored_sha256": stored_hash,
            "stored_path": str(destination),
            "correct_class": correct_class,
            "predicted_class_before": predicted_class,
            "model_score_before": float(model_score),
            "active": False,
            "added_at_utc": datetime.now(timezone.utc).isoformat(),
        }
        records.append(record)
        self._save_records(records)
        return record

    def lookup_active_correction(self, image_path: Path) -> dict | None:
        """Return an active exact-image correction, if the student taught this file."""

        image_hash = file_sha256(Path(image_path))
        return next(
            (
                record
                for record in self._load_records()
                if record.get("active")
                and image_hash in (record.get("sha256"), record.get("stored_sha256"))
            ),
            None,
        )

    def feedback_counts(self) -> dict[str, int]:
        """Count the saved student photographs in each class folder."""

        return {
            class_name: sum(
                1
                for path in self.class_directory(class_name).iterdir()
                if path.is_file() and path.suffix.lower() in IMAGE_EXTENSIONS
            )
            for class_name in self.class_names
        }

    def pending_count(self) -> int:
        """Count corrections that have not yet been activated by a learning run."""

        return sum(not bool(record.get("active")) for record in self._load_records())

    @staticmethod
    def _image_variants(image_path: Path) -> list[np.ndarray]:
        """Create modest visual variations that help one correction teach more safely."""

        with Image.open(image_path) as opened_image:
            image = (
                ImageOps.exif_transpose(opened_image)
                .convert("RGB")
                .resize((224, 224), Image.Resampling.BILINEAR)
            )

        # The variations preserve the material identity while changing orientation,
        # brightness/contrast or framing.  This is simple data augmentation.
        variants = [
            image,
            ImageOps.mirror(image),
            image.rotate(7, resample=Image.Resampling.BILINEAR, fillcolor=(245, 245, 245)),
            image.rotate(-7, resample=Image.Resampling.BILINEAR, fillcolor=(245, 245, 245)),
            ImageEnhance.Contrast(image).enhance(0.85),
            ImageEnhance.Contrast(image).enhance(1.15),
            ImageEnhance.Brightness(image).enhance(0.90),
            ImageEnhance.Brightness(image).enhance(1.10),
        ]
        return [np.asarray(variant, dtype=np.float32) / 255.0 for variant in variants]

    @staticmethod
    def _build_feature_extractor(full_model: tf.keras.Model) -> tf.keras.Model:
        """Return a sub-model from the model input to its penultimate layer."""

        import tensorflow as tf

        # Ensure Sequential models in Keras 3 have a defined symbolic input graph.
        if not getattr(full_model, "inputs", None):
            full_model.build((None, 224, 224, 3))
        try:
            inputs = full_model.inputs
            return tf.keras.Model(inputs, full_model.layers[-2].output)
        except Exception:
            # Fallback for Sequential models whose inner layers lack symbolic outputs.
            sample_input = tf.keras.Input(shape=(224, 224, 3), name="feature_input")
            x = sample_input
            for layer in full_model.layers[:-1]:
                x = layer(x)
            return tf.keras.Model(sample_input, x)

    @staticmethod
    def _remove_path(path: Path) -> None:
        """Delete a file or directory if it exists."""

        if not path.exists():
            return
        if path.is_dir():
            shutil.rmtree(path)
        else:
            path.unlink()

    def _replace_model_safely(self, temporary_model: Path, target_model: Path) -> None:
        """Atomically replace a saved model file (or SavedModel directory on older TF)."""

        if target_model.is_dir() or temporary_model.is_dir():
            self._remove_path(target_model)
            shutil.move(str(temporary_model), str(target_model))
        else:
            os.replace(temporary_model, target_model)

    def fine_tune(
        self,
        factory_model_path: Path,
        feature_bank_path: Path,
        progress: Callable[[int, str], None] | None = None,
    ) -> dict:
        """Fine-tune the final Dense layer and activate all saved corrections.

        The expensive VGG16 visual feature extractor stays frozen.  A precomputed,
        balanced feature bank provides replay examples from every class, while the
        student's corrected images receive a larger training weight.  This makes the
        lesson fast enough for ordinary CPU-only classroom computers.
        """

        import tensorflow as tf  # Delayed import keeps ordinary UI startup responsive.

        progress = progress or (lambda percentage, message: None)
        records = self._load_records()
        if not records:
            raise ValueError("Add at least one corrected image before improving the model.")
        if not Path(feature_bank_path).exists():
            raise FileNotFoundError(
                f"The balanced teaching feature bank is missing:\n{feature_bank_path}"
            )

        progress(4, "Loading the current model and balanced teaching examples…")
        current_model_path = (
            self.student_model_path if self.student_model_path.exists() else Path(factory_model_path)
        )
        if not current_model_path.exists():
            raise FileNotFoundError(f"Model file was not found: {current_model_path}")

        full_model = tf.keras.models.load_model(current_model_path, compile=False)
        if len(full_model.layers) < 2 or int(full_model.output_shape[-1]) != len(self.class_names):
            raise ValueError("This learning module requires the supplied nine-class model.")

        # The penultimate Flatten layer produces 25,088 visual features.  Only the
        # final Dense layer is trained, so the lesson is much faster than full VGG16.
        feature_model = self._build_feature_extractor(full_model)
        output_layer = full_model.layers[-1]

        with np.load(feature_bank_path) as bank:
            train_features = np.asarray(bank["train_features"], dtype=np.float32)
            train_labels = np.asarray(bank["train_labels"], dtype=np.int32)
            validation_features = np.asarray(bank["validation_features"], dtype=np.float32)
            validation_labels = np.asarray(bank["validation_labels"], dtype=np.int32)

        feature_count = int(train_features.shape[1])
        extractor_dim = int(feature_model.output_shape[-1])
        if extractor_dim != feature_count:
            raise ValueError(
                f"Feature dimension mismatch: model produces {extractor_dim}, "
                f"but feature bank has {feature_count}."
            )

        progress(12, "Creating augmented versions of student corrections…")
        feedback_images: list[np.ndarray] = []
        feedback_labels: list[int] = []
        valid_hashes: set[str] = set()
        for record in records:
            stored_path_str = record.get("stored_path", "")
            if not stored_path_str:
                continue
            stored_path = Path(stored_path_str)
            if not stored_path.exists() or record.get("correct_class") not in self.class_names:
                continue
            variants = self._image_variants(stored_path)
            feedback_images.extend(variants)
            feedback_labels.extend([self.class_names.index(record["correct_class"])] * len(variants))
            if record.get("sha256"):
                valid_hashes.add(record["sha256"])
        if not feedback_images:
            raise ValueError("No readable corrected photographs were found.")

        progress(22, "Extracting visual features from corrected photographs…")
        feedback_image_array = np.stack(feedback_images).astype(np.float32)
        feedback_features = feature_model.predict(feedback_image_array, batch_size=8, verbose=0)
        feedback_labels_array = np.asarray(feedback_labels, dtype=np.int32)

        # Build a small classifier that has exactly the same weights as the full
        # model's final layer.  Training it on stored features avoids repeating all
        # expensive convolution operations during every epoch.
        head_input = tf.keras.Input(shape=(feature_count,), name="saved_visual_features")
        head_output = tf.keras.layers.Dense(
            len(self.class_names), activation="softmax", name="student_classification"
        )(head_input)
        head_model = tf.keras.Model(head_input, head_output)
        output_weights = output_layer.get_weights()
        if len(output_weights) != len(head_model.layers[-1].get_weights()):
            raise ValueError("The final layer does not have the expected Dense weights.")
        head_model.layers[-1].set_weights(output_weights)
        head_model.compile(
            optimizer=tf.keras.optimizers.Adam(learning_rate=8e-5),
            loss="sparse_categorical_crossentropy",
            metrics=["accuracy"],
        )

        before_validation = float(head_model.evaluate(validation_features, validation_labels, verbose=0)[1])
        before_feedback_probabilities = head_model.predict(feedback_features, verbose=0)
        before_feedback_accuracy = float(
            np.mean(np.argmax(before_feedback_probabilities, axis=1) == feedback_labels_array)
        )

        combined_features = np.concatenate([train_features, feedback_features], axis=0)
        combined_labels = np.concatenate([train_labels, feedback_labels_array], axis=0)

        # Student corrections are deliberately important but balanced replay examples
        # remain present to reduce catastrophic forgetting of the other classes.
        sample_weights = np.concatenate(
            [
                np.ones(len(train_labels), dtype=np.float32),
                np.full(len(feedback_labels_array), 12.0, dtype=np.float32),
            ]
        )

        manager_progress = progress

        class EpochProgress(tf.keras.callbacks.Callback):
            def on_epoch_end(self, epoch: int, logs: dict | None = None) -> None:
                logs = logs or {}
                percentage = 32 + int(((epoch + 1) / 18) * 50)
                manager_progress(
                    percentage,
                    f"Fine-tuning final layer: epoch {epoch + 1}/18 — accuracy {logs.get('accuracy', 0):.1%}",
                )

        progress(30, "Fine-tuning the final classification layer…")
        head_model.fit(
            combined_features,
            combined_labels,
            sample_weight=sample_weights,
            validation_data=(validation_features, validation_labels),
            epochs=18,
            batch_size=32,
            shuffle=True,
            verbose=0,
            callbacks=[EpochProgress()],
        )

        progress(84, "Checking before/after performance…")
        after_validation = float(head_model.evaluate(validation_features, validation_labels, verbose=0)[1])
        after_feedback_probabilities = head_model.predict(feedback_features, verbose=0)
        after_feedback_accuracy = float(
            np.mean(np.argmax(after_feedback_probabilities, axis=1) == feedback_labels_array)
        )

        # Copy the newly learned final-layer weights back into the complete network.
        output_layer.set_weights(head_model.layers[-1].get_weights())
        if self.student_model_path.exists():
            self._remove_path(self.backup_model_path)
            if self.student_model_path.is_dir():
                shutil.copytree(self.student_model_path, self.backup_model_path)
            else:
                shutil.copy2(self.student_model_path, self.backup_model_path)

        temporary_model = self.models_directory / "student_model_new.keras"
        self._remove_path(temporary_model)
        progress(90, "Saving the personalised model safely…")
        full_model.save(temporary_model)
        self._replace_model_safely(temporary_model, self.student_model_path)

        trained_at = datetime.now(timezone.utc).isoformat()
        for record in records:
            if record.get("sha256") in valid_hashes:
                record["active"] = True
                record["last_trained_at_utc"] = trained_at
        self._save_records(records)

        report = {
            "status": "completed",
            "trained_at_utc": trained_at,
            "correction_images": len(valid_hashes),
            "augmented_feedback_examples": len(feedback_labels_array),
            "balanced_replay_examples": len(train_labels),
            "validation_examples": len(validation_labels),
            "validation_accuracy_before": before_validation,
            "validation_accuracy_after": after_validation,
            "feedback_accuracy_before": before_feedback_accuracy,
            "feedback_accuracy_after": after_feedback_accuracy,
            "student_model_path": str(self.student_model_path),
        }
        _atomic_write_text(self.training_report_path, json.dumps(report, indent=2))
        progress(100, "Learning complete. The personalised model is ready.")
        return report

    def restore_factory_model(self) -> bool:
        """Remove only the personalised model; keep feedback photographs for study."""

        removed = False
        for path in (self.student_model_path, self.backup_model_path):
            if path.exists():
                self._remove_path(path)
                removed = True
        records = self._load_records()
        for record in records:
            record["active"] = False
        self._save_records(records)
        return removed


APP_TITLE = "PIU Waste Classifier"
SOURCE_DIRECTORY = Path(__file__).resolve().parent
PROJECT_DIRECTORY = SOURCE_DIRECTORY.parent


def bundled_path(relative_path: str) -> Path:
    """Find a resource in source mode or in the PyInstaller application folder."""

    # PyInstaller exposes bundled files through _MEIPASS.  When students run the
    # readable source, resources instead live under the main project directory.
    base_directory = Path(getattr(sys, "_MEIPASS", PROJECT_DIRECTORY))
    return base_directory / relative_path


def run_packaged_self_test(output_file: Path) -> None:
    """Load the packaged model, classify a bundled image and write a JSON report."""

    image_path = bundled_path("Assets/self_test_plastic.jpg")
    try:
        classifier = WasteClassifier(bundled_path("Models/classifyWaste.h5"))
        result = classifier.predict(image_path)
        report = {
            "status": "passed",
            "test_image": str(image_path),
            "predicted_class": result["display_name"],
            "model_score_percent": result["confidence"],
        }
        _atomic_write_text(output_file, json.dumps(report, indent=2))
    except Exception as error:
        report = {
            "status": "failed",
            "test_image": str(image_path),
            "error": str(error),
        }
        _atomic_write_text(output_file, json.dumps(report, indent=2))
        raise


def run_learning_self_test(output_file: Path, data_directory: Path) -> None:
    """Exercise correction, fine-tuning, reload and exact-image recall end to end."""

    manager = StudentLearningManager(CLASS_NAMES, data_root=data_directory)
    image_path = bundled_path("Assets/self_test_plastic.jpg")
    classifier = WasteClassifier(
        bundled_path("Models/classifyWaste.h5"),
        manager.student_model_path,
        manager.lookup_active_correction,
    )
    before = classifier.predict(image_path)
    manager.add_correction(image_path, "Plastic", before["class_name"], before["confidence"])
    training_report = manager.fine_tune(
        bundled_path("Models/classifyWaste.h5"),
        bundled_path("Models/teaching_feature_bank.npz"),
    )
    classifier.reload_model()
    after = classifier.predict(image_path)
    passed = after["class_name"] == "Plastic" and after["learned_from_feedback"]
    report = {
        "status": "passed" if passed else "failed",
        "prediction_before": before["display_name"],
        "prediction_after": after["display_name"],
        "remembered_correction": after["learned_from_feedback"],
        "training": training_report,
    }
    _atomic_write_text(output_file, json.dumps(report, indent=2))
    if not passed:
        raise RuntimeError("The learning self-test did not recall the corrected class.")


class PlasticWasteApp:
    """Create and control the complete two-tab educational application."""

    def __init__(self, root: tk.Tk) -> None:
        self.root = root
        self.root.title(APP_TITLE)
        # Conservative dimensions also fit common 1366x768 classroom laptops at
        # Windows display scaling of 125%.  Panels expand when more space is available.
        self.root.geometry("1060x650")
        self.root.minsize(900, 580)
        self.root.configure(background="#eef6f1")
        self.root.protocol("WM_DELETE_WINDOW", self._on_close)

        # These attributes remember application state between button clicks.
        self.selected_image_path: Path | None = None
        self.preview_photo: ImageTk.PhotoImage | None = None
        self.teach_preview_photo: ImageTk.PhotoImage | None = None
        self.classifier: WasteClassifier | None = None
        self.learning_manager: StudentLearningManager | None = None
        self.class_names: list[str] = []
        self.last_result: dict | None = None
        self.model_ready = False
        self.busy = False
        self._closing = False

        self._configure_styles()
        self._build_interface()
        self._start_model_loading()

    def _schedule_ui(self, callback: Callable[[], None]) -> None:
        """Safely marshal a callback onto the Tkinter UI thread."""

        if self._closing:
            return
        try:
            self.root.after(0, callback)
        except (RuntimeError, tk.TclError):
            pass

    def _on_close(self) -> None:
        """Handle window closing gracefully, warning if training is active."""

        if self.busy and not messagebox.askyesno(
            "Operation in progress",
            "An AI operation is currently running. Close the application anyway?",
        ):
            return
        self._closing = True
        self.root.destroy()

    def _configure_styles(self) -> None:
        """Define a bright, consistent visual style for all controls."""

        style = ttk.Style(self.root)
        style.theme_use("clam")
        style.configure("App.TFrame", background="#eef6f1")
        style.configure("Panel.TFrame", background="#ffffff")
        style.configure("App.TLabel", background="#eef6f1", foreground="#29483b", font=("Segoe UI", 10))
        style.configure("Panel.TLabel", background="#ffffff", foreground="#29483b", font=("Segoe UI", 10))
        style.configure("Muted.TLabel", background="#ffffff", foreground="#687a72", font=("Segoe UI", 9))
        style.configure("Heading.TLabel", background="#eef6f1", foreground="#126b43", font=("Segoe UI Semibold", 21))
        style.configure("Subheading.TLabel", background="#ffffff", foreground="#126b43", font=("Segoe UI Semibold", 15))
        style.configure(
            "Primary.TButton",
            background="#167346",
            foreground="white",
            font=("Segoe UI Semibold", 10),
            padding=(16, 10),
        )
        style.map(
            "Primary.TButton",
            background=[("active", "#0f603a"), ("disabled", "#b9ccc1")],
            foreground=[("disabled", "#ffffff")],
        )
        style.configure(
            "Accent.TButton",
            background="#f4b63e",
            foreground="#27382f",
            font=("Segoe UI Semibold", 10),
            padding=(14, 9),
        )
        style.map(
            "Accent.TButton",
            background=[("active", "#dda12c"), ("disabled", "#e8e2d5")],
            foreground=[("disabled", "#7a857f")],
        )
        style.configure(
            "Secondary.TButton",
            background="#e1eee7",
            foreground="#176b46",
            font=("Segoe UI Semibold", 9),
            padding=(10, 8),
        )
        style.map(
            "Secondary.TButton",
            background=[("active", "#cfe4d8"), ("disabled", "#edf3f0")],
            foreground=[("disabled", "#8da097")],
        )
        style.configure(
            "Class.TButton",
            background="#f1f8f4",
            foreground="#24533f",
            font=("Segoe UI Semibold", 10),
            padding=(12, 10),
        )
        style.map(
            "Class.TButton",
            background=[("active", "#d9eee2"), ("disabled", "#f4f7f5")],
            foreground=[("disabled", "#94a69d")],
        )
        style.configure("TNotebook", background="#eef6f1", borderwidth=0)
        style.configure("TNotebook.Tab", font=("Segoe UI Semibold", 10), padding=(18, 9))
        # The Clam theme normally gives the selected tab extra top padding.  That
        # makes one tab jump higher/lower when students switch pages.  Explicitly
        # keep identical padding in every state so both tab labels stay aligned.
        style.map(
            "TNotebook.Tab",
            padding=[("selected", (18, 9)), ("!selected", (18, 9))],
            background=[("selected", "#ffffff"), ("!selected", "#dfeae4")],
        )

    def _build_interface(self) -> None:
        """Create the heading, Classify tab, Teach AI tab and status footer."""

        main = ttk.Frame(self.root, style="App.TFrame", padding=12)
        main.pack(fill=BOTH, expand=True)
        ttk.Label(main, text=APP_TITLE, style="Heading.TLabel").pack(anchor="w")
        ttk.Label(
            main,
            text="Classify → correct a mistake → improve the model → compare before and after",
            style="App.TLabel",
        ).pack(anchor="w", pady=(2, 7))

        self.notebook = ttk.Notebook(main)
        self.notebook.pack(fill=BOTH, expand=True)
        self.classify_tab = ttk.Frame(self.notebook, style="App.TFrame", padding=4)
        self.teach_tab = ttk.Frame(self.notebook, style="App.TFrame", padding=4)
        self.notebook.add(self.classify_tab, text="1  Classify Image")
        self.notebook.add(self.teach_tab, text="2  Teach AI / Train Model")
        self._build_classify_tab()
        self._build_teach_tab()

        footer = ttk.Frame(main, style="App.TFrame")
        footer.pack(fill=X, pady=(7, 0))
        self.status_label = ttk.Label(footer, text="Preparing the AI model…", style="App.TLabel")
        self.status_label.pack(side=LEFT)
        ttk.Button(
            footer,
            text="Open student dataset",
            style="Secondary.TButton",
            command=self._open_feedback_dataset,
        ).pack(side=RIGHT)

    def _build_classify_tab(self) -> None:
        """Build the photo chooser and prediction result panels."""

        self.classify_tab.columnconfigure(0, weight=1)
        self.classify_tab.columnconfigure(1, weight=1)
        self.classify_tab.rowconfigure(0, weight=1)

        left_panel = ttk.Frame(self.classify_tab, style="Panel.TFrame", padding=14)
        left_panel.grid(row=0, column=0, sticky="nsew", padx=(0, 8), pady=8)
        left_panel.columnconfigure(0, weight=1)
        left_panel.rowconfigure(1, weight=1)
        ttk.Label(left_panel, text="Choose a waste photograph", style="Subheading.TLabel").grid(row=0, column=0, sticky="w")
        self.preview_label = tk.Label(
            left_panel,
            text="Choose a clear JPEG or PNG image\nwith one main object.",
            bg="#edf5f0",
            fg="#567166",
            font=("Segoe UI", 13),
            justify="center",
        )
        self.preview_label.grid(row=1, column=0, sticky="nsew", pady=14)
        controls = ttk.Frame(left_panel, style="Panel.TFrame")
        controls.grid(row=2, column=0, sticky="ew")
        self.choose_button = ttk.Button(
            controls, text="Choose image", style="Secondary.TButton", command=self._choose_image
        )
        self.choose_button.pack(side=LEFT)
        self.classify_button = ttk.Button(
            controls, text="Classify image", style="Primary.TButton", command=self._classify_image, state=DISABLED
        )
        self.classify_button.pack(side=RIGHT)
        self.file_label = ttk.Label(left_panel, text="No image selected", style="Muted.TLabel")
        self.file_label.grid(row=3, column=0, sticky="w", pady=(10, 0))

        right_panel = ttk.Frame(self.classify_tab, style="Panel.TFrame", padding=14)
        right_panel.grid(row=0, column=1, sticky="nsew", padx=(8, 0), pady=8)
        right_panel.columnconfigure(0, weight=1)
        right_panel.rowconfigure(3, weight=1)
        ttk.Label(right_panel, text="Classification result", style="Subheading.TLabel").grid(row=0, column=0, sticky="w")
        self.result_label = ttk.Label(right_panel, text="Waiting for an image", style="Panel.TLabel", font=("Segoe UI Semibold", 18))
        self.result_label.grid(row=1, column=0, sticky="w", pady=(18, 3))
        self.score_label = ttk.Label(right_panel, text="", style="Muted.TLabel")
        self.score_label.grid(row=2, column=0, sticky="w")
        self.explanation = tk.Text(
            right_panel,
            height=8,
            wrap="word",
            state=DISABLED,
            background="#f7fbf8",
            foreground="#29483b",
            relief="flat",
            font=("Segoe UI", 10),
            padx=13,
            pady=13,
        )
        self.explanation.grid(row=3, column=0, sticky="nsew", pady=(14, 12))
        self.wrong_button = ttk.Button(
            right_panel,
            text="Prediction wrong? Teach the correct class",
            style="Accent.TButton",
            command=self._teach_current_image,
            state=DISABLED,
        )
        self.wrong_button.grid(row=4, column=0, sticky="ew")
        self.dataset_label = ttk.Label(right_panel, text="Student feedback dataset: preparing…", style="Muted.TLabel")
        self.dataset_label.grid(row=5, column=0, sticky="w", pady=(10, 0))

    def _build_teach_tab(self) -> None:
        """Build category cards, progress display and model-management controls."""

        self.teach_tab.columnconfigure(0, weight=1)
        self.teach_tab.columnconfigure(1, weight=2)
        self.teach_tab.rowconfigure(0, weight=1)

        info_panel = ttk.Frame(self.teach_tab, style="Panel.TFrame", padding=14)
        info_panel.grid(row=0, column=0, sticky="nsew", padx=(0, 8), pady=8)
        info_panel.columnconfigure(0, weight=1)
        info_panel.rowconfigure(1, weight=1)
        ttk.Label(info_panel, text="Teach this photograph", style="Subheading.TLabel").grid(row=0, column=0, sticky="w")
        self.teach_preview = tk.Label(
            info_panel,
            text="Classify an image first, then use\n‘Prediction wrong?’",
            bg="#edf5f0",
            fg="#567166",
            font=("Segoe UI", 11),
            justify="center",
            height=6,
        )
        self.teach_preview.grid(row=1, column=0, sticky="nsew", pady=14)
        self.teach_image_label = ttk.Label(info_panel, text="No correction selected", style="Panel.TLabel", wraplength=310)
        self.teach_image_label.grid(row=2, column=0, sticky="w")
        self.before_prediction_label = ttk.Label(info_panel, text="", style="Muted.TLabel", wraplength=310)
        self.before_prediction_label.grid(row=3, column=0, sticky="w", pady=(5, 14))
        ttk.Label(
            info_panel,
            text=(
                "Teaching note\n\nOne photograph can be remembered, but several varied photographs "
                "are needed for reliable general learning. Add different angles, lighting and backgrounds."
            ),
            style="Panel.TLabel",
            wraplength=315,
            justify="left",
        ).grid(row=4, column=0, sticky="nw")

        learning_panel = ttk.Frame(self.teach_tab, style="Panel.TFrame", padding=14)
        learning_panel.grid(row=0, column=1, sticky="nsew", padx=(8, 0), pady=8)
        learning_panel.columnconfigure(0, weight=1)
        learning_panel.rowconfigure(3, weight=1)
        ttk.Label(learning_panel, text="What is the correct category?", style="Subheading.TLabel").grid(row=0, column=0, sticky="w")
        ttk.Label(
            learning_panel,
            text="Choose one card. The image will be copied into that class in the local student dataset.",
            style="Panel.TLabel",
        ).grid(row=1, column=0, sticky="w", pady=(3, 12))
        self.class_button_frame = ttk.Frame(learning_panel, style="Panel.TFrame")
        self.class_button_frame.grid(row=2, column=0, sticky="ew")
        for column in range(3):
            self.class_button_frame.columnconfigure(column, weight=1)

        self.class_buttons: list[ttk.Button] = []
        # Buttons are populated after CLASS_NAMES supplies the exact model order.

        progress_area = ttk.Frame(learning_panel, style="Panel.TFrame")
        progress_area.grid(row=3, column=0, sticky="nsew", pady=(16, 0))
        progress_area.columnconfigure(0, weight=1)
        progress_area.rowconfigure(2, weight=1)
        self.feedback_summary_label = ttk.Label(progress_area, text="No student corrections yet.", style="Panel.TLabel")
        self.feedback_summary_label.grid(row=0, column=0, sticky="w")
        self.training_progress = ttk.Progressbar(progress_area, mode="determinate", maximum=100)
        self.training_progress.grid(row=1, column=0, sticky="ew", pady=(9, 8))
        self.training_log = tk.Text(
            progress_area,
            height=4,
            wrap="word",
            state=DISABLED,
            background="#f7fbf8",
            foreground="#29483b",
            relief="flat",
            font=("Segoe UI", 9),
            padx=10,
            pady=8,
        )
        self.training_log.grid(row=2, column=0, sticky="nsew")
        actions = ttk.Frame(progress_area, style="Panel.TFrame")
        actions.grid(row=3, column=0, sticky="ew", pady=(12, 0))
        self.improve_button = ttk.Button(
            actions, text="Improve AI Model", style="Primary.TButton", command=self._start_fine_tuning, state=DISABLED
        )
        self.improve_button.pack(side=RIGHT)
        self.restore_button = ttk.Button(
            actions,
            text="Restore factory model",
            style="Secondary.TButton",
            command=self._restore_factory_model,
            state=DISABLED,
        )
        self.restore_button.pack(side=LEFT)

    def _start_model_loading(self) -> None:
        """Import TensorFlow and load resources without freezing the visible window."""

        def worker() -> None:
            try:
                manager = StudentLearningManager(CLASS_NAMES)
                classifier = WasteClassifier(
                    bundled_path("Models/classifyWaste.h5"),
                    manager.student_model_path,
                    manager.lookup_active_correction,
                )
            except Exception as error:
                self._schedule_ui(lambda err=error: self._model_failed(err))
                return
            self._schedule_ui(lambda: self._model_loaded(manager, classifier))

        threading.Thread(target=worker, daemon=True).start()

    def _model_loaded(self, manager: StudentLearningManager, classifier: WasteClassifier) -> None:
        """Finish UI setup after the model and learning manager are ready."""

        self.class_names = CLASS_NAMES
        self.learning_manager = manager
        self.classifier = classifier
        self.model_ready = True

        for button in self.class_buttons:
            button.destroy()
        self.class_buttons.clear()
        for index, class_name in enumerate(self.class_names):
            display_name = MATERIAL_INFORMATION[class_name]["display_name"]
            button = ttk.Button(
                self.class_button_frame,
                text=display_name,
                style="Class.TButton",
                command=lambda selected=class_name: self._save_correction(selected),
            )
            button.grid(row=index // 3, column=index % 3, sticky="ew", padx=4, pady=4)
            self.class_buttons.append(button)

        model_type = (
            "personalised student model"
            if self.classifier.loaded_model_path == self.learning_manager.student_model_path
            else "factory model"
        )
        self.status_label.configure(text=f"Ready — using {model_type}.")
        self._refresh_feedback_summary()
        self._update_controls()

    def _model_failed(self, error: Exception) -> None:
        self.status_label.configure(text="The AI model could not be loaded.")
        messagebox.showerror("Model unavailable", f"The model could not be loaded.\n\n{error}")

    def _reset_teach_preview(self) -> None:
        """Clear the Teach tab preview when switching images or resetting."""

        self.teach_preview_photo = None
        self.teach_preview.configure(
            image="",
            text="Classify an image first, then use\n‘Prediction wrong?’",
            height=6,
        )
        self.teach_image_label.configure(text="No correction selected")
        self.before_prediction_label.configure(text="")

    def _choose_image(self) -> None:
        """Let the student select a JPEG/PNG and show a correctly oriented preview."""

        if self.busy:
            return
        selected = filedialog.askopenfilename(
            title="Choose a waste image",
            filetypes=(("Images", "*.jpg *.jpeg *.png"), ("JPEG", "*.jpg *.jpeg"), ("PNG", "*.png")),
        )
        if not selected:
            return
        image_path = Path(selected)
        try:
            with Image.open(image_path) as opened_image:
                preview = ImageOps.exif_transpose(opened_image).convert("RGB")
                preview.thumbnail((510, 410))
        except (UnidentifiedImageError, OSError):
            messagebox.showerror("Unsupported image", "Please choose a readable JPEG or PNG photograph.")
            return

        self.preview_photo = ImageTk.PhotoImage(preview)
        self.preview_label.configure(image=self.preview_photo, text="")
        self.selected_image_path = image_path
        self.last_result = None
        self.file_label.configure(text=image_path.name)
        self.result_label.configure(text="Ready to classify")
        self.score_label.configure(text="")
        self._set_explanation("Click ‘Classify image’ to ask the current local model.")
        self._reset_teach_preview()
        self._update_controls()

    def _update_controls(self) -> None:
        """Enable only actions that are safe in the current state."""

        can_choose = not self.busy
        can_classify = self.model_ready and self.selected_image_path is not None and not self.busy
        can_teach = self.model_ready and self.last_result is not None and not self.busy
        pending = self.learning_manager.pending_count() if self.learning_manager else 0
        can_improve = self.model_ready and pending > 0 and not self.busy
        can_restore = self.model_ready and not self.busy

        self.choose_button.configure(state="normal" if can_choose else DISABLED)
        self.classify_button.configure(state="normal" if can_classify else DISABLED)
        self.wrong_button.configure(state="normal" if can_teach else DISABLED)
        self.improve_button.configure(state="normal" if can_improve else DISABLED)
        self.restore_button.configure(state="normal" if can_restore else DISABLED)
        for button in self.class_buttons:
            button.configure(state="normal" if can_teach else DISABLED)

    def _classify_image(self) -> None:
        """Run prediction in a worker thread."""

        if self.busy or not self.model_ready or self.selected_image_path is None or self.classifier is None:
            return
        image_path = self.selected_image_path
        self.busy = True
        self.status_label.configure(text="Classifying locally…")
        self.result_label.configure(text="Classifying…")
        self._update_controls()

        def worker() -> None:
            try:
                result = self.classifier.predict(image_path)
            except Exception as error:
                self._schedule_ui(lambda err=error: self._prediction_failed(err))
                return
            self._schedule_ui(lambda res=result: self._show_result(res))

        threading.Thread(target=worker, daemon=True).start()

    def _show_result(self, result: dict) -> None:
        """Display model output and expose the correction workflow."""

        self.last_result = result
        self.result_label.configure(text=f"Classified as {result['display_name']}")
        if result["learned_from_feedback"]:
            score_text = (
                f"Learned correction recalled • underlying model score for this class: {result['confidence']:.2f}%"
            )
            learning_note = (
                "This exact photograph was recalled from student feedback. Try new photographs of the same "
                "material to test whether fine-tuning generalised beyond memorisation."
            )
        else:
            score_text = f"Model score: {result['confidence']:.2f}%"
            learning_note = (
                "The score is not guaranteed certainty. If the answer is wrong, teach the true class and "
                "then run Improve AI Model."
            )
        self.score_label.configure(text=score_text)
        self._set_explanation(
            f"ABOUT THIS MATERIAL\n\n{result['description']}\n\n"
            f"SEPARATION / RECYCLING ADVICE\n\n{result['advice']}\n\n"
            f"LEARNING NOTE\n\n{learning_note}"
        )
        self.status_label.configure(text="Classification complete.")
        self.busy = False
        self._update_controls()

    def _prediction_failed(self, error: Exception) -> None:
        self.result_label.configure(text="Unable to classify this image")
        self._set_explanation("Try a clear, well-lit JPEG or PNG containing one main item.")
        self.status_label.configure(text="Classification failed.")
        self.busy = False
        self._update_controls()
        messagebox.showerror("Classification failed", str(error))

    def _teach_current_image(self) -> None:
        """Move the current image and its old result into the Teach AI tab."""

        if self.busy or self.selected_image_path is None or self.last_result is None:
            return
        try:
            with Image.open(self.selected_image_path) as opened_image:
                preview = ImageOps.exif_transpose(opened_image).convert("RGB")
                preview.thumbnail((330, 265))
        except OSError:
            return
        self.teach_preview_photo = ImageTk.PhotoImage(preview)
        # Reset height=0 so Tkinter does not treat height=6 as 6 pixels for an image.
        self.teach_preview.configure(image=self.teach_preview_photo, text="", height=0)
        self.teach_image_label.configure(text=self.selected_image_path.name)
        self.before_prediction_label.configure(
            text=f"Current answer: {self.last_result['display_name']} ({self.last_result['confidence']:.1f}%)"
        )
        self.notebook.select(self.teach_tab)
        self.status_label.configure(text="Choose the photograph's true category.")
        self._update_controls()

    def _save_correction(self, correct_class: str) -> None:
        """Copy the current image into its selected class in the feedback dataset."""

        if self.busy or not self.learning_manager or not self.selected_image_path or not self.last_result:
            return
        try:
            self.learning_manager.add_correction(
                self.selected_image_path,
                correct_class,
                self.last_result["class_name"],
                self.last_result["confidence"],
            )
        except Exception as error:
            messagebox.showerror("Could not add correction", str(error))
            return

        display_name = MATERIAL_INFORMATION[correct_class]["display_name"]
        self._append_training_log(
            f"Added ‘{self.selected_image_path.name}’ to {display_name}. This correction is pending training."
        )
        self.status_label.configure(text=f"Correction saved as {display_name}. Click Improve AI Model.")
        self._refresh_feedback_summary()
        self._update_controls()

    def _refresh_feedback_summary(self) -> None:
        """Show how the student's nine-class dataset is growing."""

        if not self.learning_manager:
            return
        counts = self.learning_manager.feedback_counts()
        total = sum(counts.values())
        pending = self.learning_manager.pending_count()
        self.dataset_label.configure(text=f"Student feedback dataset: {total} images • {pending} awaiting learning")
        self.feedback_summary_label.configure(
            text=f"Saved corrections: {total} across 9 classes • Pending training: {pending}"
        )

    def _start_fine_tuning(self) -> None:
        """Confirm and start balanced final-layer fine-tuning in a worker."""

        if self.busy or not self.learning_manager or not self.classifier:
            return
        pending = self.learning_manager.pending_count()
        if pending < 1:
            messagebox.showinfo("Nothing to train", "Add at least one corrected image first.")
            return
        if not messagebox.askyesno(
            "Improve AI Model",
            f"Use {pending} new correction(s) plus balanced replay examples to fine-tune the model?\n\n"
            "This usually takes a few minutes on a CPU-only computer. Keep the app open.",
        ):
            return

        selected_image = self.selected_image_path
        self.busy = True
        self.training_progress["value"] = 0
        self._append_training_log("Starting a new learning run…")
        self.status_label.configure(text="Improving the model — please keep the app open…")
        self._update_controls()

        def progress(percentage: int, message: str) -> None:
            self._schedule_ui(lambda p=percentage, m=message: self._update_training_progress(p, m))

        def worker() -> None:
            try:
                report = self.learning_manager.fine_tune(
                    bundled_path("Models/classifyWaste.h5"),
                    bundled_path("Models/teaching_feature_bank.npz"),
                    progress,
                )
                self.classifier.reload_model()
            except Exception as error:
                self._schedule_ui(lambda err=error: self._training_failed(err))
                return

            after_result = None
            if selected_image and selected_image.exists():
                try:
                    after_result = self.classifier.predict(selected_image)
                except Exception:
                    after_result = None
            self._schedule_ui(lambda rep=report, res=after_result: self._training_complete(rep, res))

        threading.Thread(target=worker, daemon=True).start()

    def _update_training_progress(self, percentage: int, message: str) -> None:
        self.training_progress["value"] = percentage
        self.status_label.configure(text=message)
        self._append_training_log(message)

    def _training_complete(self, report: dict, after_result: dict | None) -> None:
        """Show before/after metrics and the corrected image's new answer."""

        self.busy = False
        self.training_progress["value"] = 100
        self._refresh_feedback_summary()
        self._append_training_log(
            "Learning complete: validation accuracy "
            f"{report['validation_accuracy_before']:.1%} → {report['validation_accuracy_after']:.1%}; "
            f"feedback examples {report['feedback_accuracy_before']:.1%} → {report['feedback_accuracy_after']:.1%}."
        )
        self.status_label.configure(text="Learning complete. The personalised model is active.")
        if after_result:
            self._show_result(after_result)
            after_text = after_result["display_name"]
        else:
            after_text = "No image selected"
        messagebox.showinfo(
            "AI learning completed",
            "The final classification layer was fine-tuned and exact corrections were activated.\n\n"
            f"Balanced validation: {report['validation_accuracy_before']:.1%} → {report['validation_accuracy_after']:.1%}\n"
            f"Corrected examples: {report['feedback_accuracy_before']:.1%} → {report['feedback_accuracy_after']:.1%}\n"
            f"Current image now: {after_text}\n\n"
            "Next experiment: test a different photograph of the same object class.",
        )
        self._update_controls()

    def _training_failed(self, error: Exception) -> None:
        self.busy = False
        self.training_progress["value"] = 0
        self.status_label.configure(text="Learning did not finish; the previous model remains available.")
        self._append_training_log(f"Learning stopped safely: {error}")
        self._update_controls()
        messagebox.showerror("Learning could not finish", f"The previous model was kept.\n\n{error}")

    def _restore_factory_model(self) -> None:
        """Return to factory weights without deleting the student's photographs."""

        if self.busy or not self.learning_manager or not self.classifier:
            return
        if not messagebox.askyesno(
            "Restore factory model",
            "Restore the original model? Student photographs will remain in the feedback dataset.",
        ):
            return
        self.learning_manager.restore_factory_model()
        try:
            self.classifier.reload_model()
        except Exception as error:
            messagebox.showerror("Restore failed", str(error))
            return

        self.last_result = None
        self.training_progress["value"] = 0
        self.result_label.configure(
            text="Ready to classify" if self.selected_image_path else "Waiting for an image"
        )
        self.score_label.configure(text="")
        self._set_explanation(
            "Factory model restored. Click ‘Classify image’ to re-evaluate the selected photo."
            if self.selected_image_path
            else ""
        )
        self._reset_teach_preview()
        self.status_label.configure(text="Factory model restored. Saved corrections are now pending again.")
        self._refresh_feedback_summary()
        self._update_controls()

    def _open_feedback_dataset(self) -> None:
        """Open the writable student class folders in the system file manager."""

        if not self.learning_manager:
            return
        folder = self.learning_manager.feedback_directory
        if hasattr(os, "startfile"):
            os.startfile(folder)
        elif sys.platform == "darwin":
            subprocess.Popen(["open", str(folder)])
        else:
            subprocess.Popen(["xdg-open", str(folder)])

    def _append_training_log(self, message: str) -> None:
        self.training_log.configure(state="normal")
        self.training_log.insert(END, f"• {message}\n")
        self.training_log.see(END)
        self.training_log.configure(state=DISABLED)

    def _set_explanation(self, text: str) -> None:
        self.explanation.configure(state="normal")
        self.explanation.delete("1.0", END)
        self.explanation.insert("1.0", text)
        self.explanation.configure(state=DISABLED)


def main() -> None:
    """Create the window and keep it open until the student closes it."""

    root = tk.Tk()
    try:
        icon_path = bundled_path("Assets/piu_recycling_icon.ico")
        if icon_path.exists():
            root.iconbitmap(icon_path)
    except tk.TclError:
        pass
    PlasticWasteApp(root)
    root.mainloop()


if __name__ == "__main__":
    # Hidden self-test modes verify the finished EXE without opening the interface.
    if len(sys.argv) == 3 and sys.argv[1] == "--self-test":
        run_packaged_self_test(Path(sys.argv[2]).resolve())
    elif len(sys.argv) == 4 and sys.argv[1] == "--learning-self-test":
        run_learning_self_test(Path(sys.argv[2]).resolve(), Path(sys.argv[3]).resolve())
    else:
        main()
