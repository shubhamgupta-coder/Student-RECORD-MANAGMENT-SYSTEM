@echo off
title Student Record Management System (SRMS)
color 0b
cd /d "%~dp0"

echo =================================================================
echo       STARTING STUDENT RECORD MANAGEMENT SYSTEM (SRMS)
echo       Authors: Shubham Gupta (45) and Swapnil Yadav (46)
echo =================================================================
echo.

if not exist srms.exe (
    if exist bin\srms.exe (
        copy /Y bin\srms.exe srms.exe >nul 2>&1
    ) else (
        echo [INFO] Executable not found. Compiling application first...
        call build.bat
    )
)

if exist srms.exe (
    cls
    .\srms.exe
) else if exist bin\srms.exe (
    cls
    .\bin\srms.exe
) else (
    echo [ERROR] Unable to locate or compile srms.exe.
)

echo.
echo =================================================================
echo Application session ended.
pause
