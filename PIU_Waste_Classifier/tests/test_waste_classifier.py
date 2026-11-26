"""Unit tests for PIU Waste Classifier core logic."""

from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path

import numpy as np
from PIL import Image

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "Source_Code"))
import piu_waste_classifier as app  # noqa: E402


class DummyModel:
    """Minimal stand-in for a loaded Keras 9-class model."""

    output_shape = (None, len(app.CLASS_NAMES))

    def __init__(self, target_index: int = 0) -> None:
        self.target_index = target_index

    def predict(self, batch: np.ndarray, verbose: int = 0) -> np.ndarray:
        assert batch.shape == (1, 224, 224, 3)
        probs = np.full((1, len(app.CLASS_NAMES)), 0.05, dtype=np.float32)
        probs[0, self.target_index] = 0.60
        return probs


class WasteClassifierTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temp_dir = tempfile.TemporaryDirectory()
        self.root = Path(self.temp_dir.name)
        self.manager = app.StudentLearningManager(app.CLASS_NAMES, data_root=self.root / "data")

    def tearDown(self) -> None:
        self.temp_dir.cleanup()

    def _make_sample_image(self, name: str = "sample.jpg", size: tuple[int, int] = (40, 20)) -> Path:
        path = self.root / name
        img = Image.new("RGB", size, color=(30, 140, 80))
        img.save(path, "JPEG")
        return path

    def test_creates_nine_class_folders_with_corrected_spelling(self) -> None:
        counts = self.manager.feedback_counts()
        self.assertEqual(len(counts), 9)
        self.assertTrue((self.manager.feedback_directory / "light_bulbs").is_dir())
        self.assertFalse((self.manager.feedback_directory / "light_blubs").exists())

    def test_reclassifying_existing_image_moves_file_and_updates_path(self) -> None:
        img = self._make_sample_image()
        first = self.manager.add_correction(img, "Paper", "Plastic", 80.0)
        first_path = Path(first["stored_path"])
        self.assertTrue(first_path.exists())
        self.assertEqual(first_path.parent.name, "paper")

        second = self.manager.add_correction(img, "Plastic", "Paper", 65.0)
        second_path = Path(second["stored_path"])
        self.assertTrue(second_path.exists())
        self.assertEqual(second_path.parent.name, "plastic")
        self.assertFalse(first_path.exists())
        self.assertEqual(self.manager.feedback_counts()["paper"], 0)
        self.assertEqual(self.manager.feedback_counts()["plastic"], 1)

    def test_lookup_active_correction_matches_original_and_stored_copy(self) -> None:
        img = self._make_sample_image()
        record = self.manager.add_correction(img, "Metal", "Glass", 51.0)
        self.assertIsNone(self.manager.lookup_active_correction(img))

        records = json.loads(self.manager.records_path.read_text(encoding="utf-8"))
        records[0]["active"] = True
        self.manager._save_records(records)

        self.assertIsNotNone(self.manager.lookup_active_correction(img))
        self.assertIsNotNone(self.manager.lookup_active_correction(Path(record["stored_path"])))

    def test_restore_factory_model_keeps_images_and_deactivates_records(self) -> None:
        img = self._make_sample_image()
        self.manager.add_correction(img, "Glass", "Plastic", 70.0)
        records = json.loads(self.manager.records_path.read_text(encoding="utf-8"))
        records[0]["active"] = True
        self.manager._save_records(records)

        self.manager.student_model_path.write_text("dummy", encoding="utf-8")
        self.assertTrue(self.manager.restore_factory_model())
        self.assertFalse(self.manager.student_model_path.exists())
        self.assertEqual(self.manager.pending_count(), 1)
        self.assertEqual(self.manager.feedback_counts()["glass"], 1)

    def test_prepare_image_applies_exif_orientation(self) -> None:
        # 40x20 image tagged with EXIF Orientation=6 (Rotate 270 CW / 90 CCW -> portrait)
        img_path = self.root / "exif.jpg"
        img = Image.new("RGB", (40, 20), color=(255, 0, 0))
        exif = img.getexif()
        exif[274] = 6  # Orientation tag
        img.save(img_path, "JPEG", exif=exif)

        batch = app.WasteClassifier.prepare_image(img_path)
        self.assertEqual(batch.shape, (1, 224, 224, 3))
        self.assertEqual(batch.dtype, np.float32)
        self.assertGreaterEqual(float(batch.min()), 0.0)
        self.assertLessEqual(float(batch.max()), 1.0)

    def test_predict_ignores_invalid_correction_class(self) -> None:
        img = self._make_sample_image()
        classifier = object.__new__(app.WasteClassifier)
        classifier.model = DummyModel(target_index=app.CLASS_NAMES.index("Paper"))
        classifier.correction_lookup = lambda _p: {"correct_class": "InvalidClass"}
        classifier._lock = app.threading.RLock()

        result = classifier.predict(img)
        self.assertEqual(result["class_name"], "Paper")
        self.assertFalse(result["learned_from_feedback"])


if __name__ == "__main__":
    unittest.main()
