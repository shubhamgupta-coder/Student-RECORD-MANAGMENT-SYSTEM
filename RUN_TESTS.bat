@echo off
title SRMS - Automated Test Suite (TC-01 to TC-07)
color 0a
cd /d "%~dp0"

echo =================================================================
echo       RUNNING AUTOMATED TEST SUITE (TC-01 TO TC-07)
echo       Authors: Shubham Gupta (45) and Swapnil Yadav (46)
echo =================================================================
echo.

if not exist test_suite.exe (
    if exist bin\test_suite.exe (
        copy /Y bin\test_suite.exe test_suite.exe >nul 2>&1
    ) else (
        echo [INFO] Test executable not found. Compiling first...
        call build.bat
    )
)

if exist test_suite.exe (
    .\test_suite.exe
) else if exist bin\test_suite.exe (
    .\bin\test_suite.exe
) else (
    echo [ERROR] Unable to run test suite.
)

echo.
echo =================================================================
echo Test execution complete.
pause
