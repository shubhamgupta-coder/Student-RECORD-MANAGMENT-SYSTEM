@echo off
echo =================================================================
echo       BUILDING STUDENT RECORD MANAGEMENT SYSTEM (SRMS)
echo       Authors: Shubham Gupta (45) and Swapnil Yadav (46)
echo =================================================================

if not exist bin mkdir bin
if not exist reports mkdir reports

echo [1/2] Compiling main SRMS Application...
g++ -std=c++17 -Wall -O2 -Iinclude src/main.cpp src/User.cpp src/Course.cpp src/Student.cpp src/Enrollment.cpp src/Mark.cpp src/Attendance.cpp src/ReportService.cpp src/StudentRepository.cpp -o bin/srms.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile bin\srms.exe
    exit /b %ERRORLEVEL%
)
echo [SUCCESS] Application compiled to bin\srms.exe!
copy /Y bin\srms.exe srms.exe >nul 2>&1

echo.
echo [2/2] Compiling Automated Test Suite...
g++ -std=c++17 -Wall -Iinclude tests/test_srms.cpp src/User.cpp src/Course.cpp src/Student.cpp src/Enrollment.cpp src/Mark.cpp src/Attendance.cpp src/ReportService.cpp src/StudentRepository.cpp -o bin/test_suite.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile bin\test_suite.exe
    exit /b %ERRORLEVEL%
)
echo [SUCCESS] Test Suite compiled to bin\test_suite.exe!
copy /Y bin\test_suite.exe test_suite.exe >nul 2>&1

echo.
echo =================================================================
echo Build Completed Successfully!
echo To run the application:  .\bin\srms.exe (or .\srms.exe)
echo To run test suite:       .\bin\test_suite.exe (or .\test_suite.exe)
echo =================================================================
