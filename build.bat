@echo off
echo =================================================================
echo       BUILDING STUDENT RECORD MANAGEMENT SYSTEM (SRMS)
echo          Authors: Shubham Gupta (45) & Swapnil Yadav (46)
echo =================================================================

if not exist bin mkdir bin
if not exist reports mkdir reports

echo [1/2] Compiling main SRMS Application...
g++ -std=c++17 -Wall -O2 -Iinclude src/main.cpp src/User.cpp src/Course.cpp src/Student.cpp src/Enrollment.cpp src/Mark.cpp src/Attendance.cpp src/ReportService.cpp src/StudentRepository.cpp -o srms.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile srms.exe
    exit /b %ERRORLEVEL%
)
echo [SUCCESS] Application compiled to srms.exe!

echo.
echo [2/2] Compiling Automated Test Suite...
g++ -std=c++17 -Wall -Iinclude tests/test_srms.cpp src/User.cpp src/Course.cpp src/Student.cpp src/Enrollment.cpp src/Mark.cpp src/Attendance.cpp src/ReportService.cpp src/StudentRepository.cpp -o test_suite.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile test_suite.exe
    exit /b %ERRORLEVEL%
)
echo [SUCCESS] Test Suite compiled to test_suite.exe!

echo.
echo =================================================================
echo Build Completed Successfully!
echo To run the application:  .\srms.exe
echo To run test suite:       .\test_suite.exe
echo =================================================================
