@echo off
setlocal
cd /d "%~dp0"

if not exist "Models\classifyWaste.h5" (
    echo [ERROR] Models\classifyWaste.h5 not found.
    echo Place classifyWaste.h5 and teaching_feature_bank.npz in the Models folder first.
    pause
    exit /b 1
)

if not exist ".venv\Scripts\python.exe" (
    echo Creating virtual environment in .venv...
    python -m venv .venv
    ".venv\Scripts\python.exe" -m pip install --upgrade pip
    ".venv\Scripts\python.exe" -m pip install -r requirements.txt
)

echo Building standalone Windows executable with PyInstaller...
".venv\Scripts\python.exe" -m PyInstaller ^
    --noconfirm ^
    --clean ^
    --windowed ^
    --name "PIU Waste Classifier" ^
    --icon "Assets\piu_recycling_icon.ico" ^
    --add-data "Models;Models" ^
    --add-data "Assets;Assets" ^
    "Source_Code\piu_waste_classifier.py"

if errorlevel 1 (
    echo [ERROR] PyInstaller build failed.
    pause
    exit /b 1
)

echo.
echo Build complete! Output is in: dist\PIU Waste Classifier\
pause
endlocal
