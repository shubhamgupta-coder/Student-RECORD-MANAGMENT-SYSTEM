# PIU Waste Classifier

Educational Windows desktop application for classifying waste photographs, collecting student corrections into a local feedback dataset, and fine-tuning the final classification layer.

## Folder Layout

```text
PIU_Waste_Classifier/
├── Source_Code/
│   └── piu_waste_classifier.py   # Complete single-file application
├── Models/
│   ├── classifyWaste.h5          # Required: 9-class factory model (place here)
│   └── teaching_feature_bank.npz # Required for "Improve AI Model"
├── Assets/
│   └── piu_recycling_icon.ico    # Application icon
├── tests/
│   └── test_waste_classifier.py  # Unit tests for learning manager & classifier
├── requirements.txt
├── RUN_APP.bat                   # Launch the desktop app
└── BUILD_EXE.bat                 # Package standalone Windows app with PyInstaller
```

## Setup & Run

1. Place `classifyWaste.h5` (and `teaching_feature_bank.npz` for fine-tuning) inside `Models/`.
2. Double-click `RUN_APP.bat`, or run manually:
   ```bat
   python -m venv .venv
   .venv\Scripts\activate
   pip install -r requirements.txt
   python Source_Code\piu_waste_classifier.py
   ```

## Run Unit Tests

```bat
python -m unittest discover -s tests -v
```

## Build Standalone Windows App (.exe)

Double-click `BUILD_EXE.bat` to generate `dist\PIU Waste Classifier\PIU Waste Classifier.exe` using PyInstaller.
