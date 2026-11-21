#include <iostream>
#include <iomanip>
#include <cassert>
#include <string>
#include <vector>
#include "../include/StudentRepository.h"
#include "../include/User.h"
#include "../include/Student.h"
#include "../include/Course.h"
#include "../include/Enrollment.h"
#include "../include/Mark.h"
#include "../include/Attendance.h"
#include "../include/ReportService.h"
#include "../include/Utils.h"

struct TestResult {
    std::string testCase;
    std::string description;
    std::string expected;
    std::string actual;
    bool passed;
};

int main() {
    std::vector<TestResult> results;
    std::cout << "\n=======================================================================\n";
    std::cout << "          STUDENT RECORD MANAGEMENT SYSTEM - AUTOMATED TEST SUITE      \n";
    std::cout << "        Executing Verification Plan: TC-01 to TC-07 from Report        \n";
    std::cout << "        Authors: Shubham Gupta (45) & Swapnil Yadav (46)               \n";
    std::cout << "=======================================================================\n\n";

    StudentRepository repo("data");
    ReportService reportService(repo);

    // ---------------------------------------------------------
    // TC-01: Valid Login
    // ---------------------------------------------------------
    {
        User* user = repo.findUserByUsername("admin");
        bool loginSuccess = (user != nullptr && user->login("admin123"));
        std::string actual = loginSuccess ? "Dashboard opens (Access Granted)" : "Access Denied";
        results.push_back({
            "TC-01",
            "Valid Login: Correct username & password",
            "Dashboard opens",
            actual,
            loginSuccess
        });
    }

    // ---------------------------------------------------------
    // TC-02: Invalid Login
    // ---------------------------------------------------------
    {
        User* user = repo.findUserByUsername("admin");
        bool badLogin = (user != nullptr && user->login("incorrectPassword!@#"));
        std::string actual = (!badLogin) ? "Access denied with a clear message" : "Dashboard opened erroneously";
        results.push_back({
            "TC-02",
            "Invalid Login: Incorrect password",
            "Access denied with a clear message",
            actual,
            !badLogin
        });
    }

    // ---------------------------------------------------------
    // TC-03: Add Student (Valid details)
    // ---------------------------------------------------------
    {
        std::string testId = "S199";
        // Remove if previously exists to ensure clean test
        Student newStudent(testId, "Rohan Verma", "2003-04-12", "rohan@univ.edu", "9123456780", "12 Main St, Delhi", "CS101", "Active");
        std::string err;
        bool added = repo.add(newStudent, err);
        Student* found = repo.findById(testId);
        bool testPassed = added && (found != nullptr && found->getFullName() == "Rohan Verma");
        std::string actual = testPassed ? "Record saved and searchable" : ("Failed to save: " + err);
        results.push_back({
            "TC-03",
            "Add Student: Complete valid form with new ID",
            "Record saved and searchable",
            actual,
            testPassed
        });
    }

    // ---------------------------------------------------------
    // TC-04: Duplicate Student ID
    // ---------------------------------------------------------
    {
        Student duplicateStudent("S101", "Duplicate Test", "2003-01-01", "dup@univ.edu", "9123456789", "Camp", "CS101", "Active");
        std::string err;
        bool added = repo.add(duplicateStudent, err);
        bool testPassed = (!added && err.find("already exists") != std::string::npos);
        std::string actual = (!added) ? "System blocks duplicate and explains why" : "System allowed duplicate ID!";
        results.push_back({
            "TC-04",
            "Duplicate ID: Reuse existing student ID",
            "System blocks duplicate and explains why",
            actual,
            testPassed
        });
    }

    // ---------------------------------------------------------
    // TC-05: Update Profile
    // ---------------------------------------------------------
    {
        Student* s = repo.findById("S101");
        bool testPassed = false;
        std::string actual = "Record not found";
        if (s) {
            std::string updatedPhone = "9988776655";
            std::string updatedAddr = "New Faculty Hostel, Room 12";
            s->updateProfile("", "", "", updatedPhone, updatedAddr, "");
            std::string updateErr;
            if (repo.update(*s, updateErr)) {
                Student* reloaded = repo.findById("S101");
                if (reloaded && reloaded->getPhone() == updatedPhone && reloaded->getAddress() == updatedAddr) {
                    testPassed = true;
                    actual = "Updated value is shown after saving";
                } else {
                    actual = "Value not persisted";
                }
            } else {
                actual = updateErr;
            }
        }
        results.push_back({
            "TC-05",
            "Update Profile: Change phone or address",
            "Updated value is shown after saving",
            actual,
            testPassed
        });
    }

    // ---------------------------------------------------------
    // TC-06: Invalid Marks
    // ---------------------------------------------------------
    {
        Mark invalidMark("M_INV", "S101", "Operating Systems", "Final Exam", 110.0, 100.0);
        std::string mErr;
        bool added = repo.addMark(invalidMark, mErr);
        bool testPassed = (!added && mErr.find("exceeds maximum") != std::string::npos);
        std::string actual = (!added) ? "Validation error; record not saved" : "Invalid mark was saved!";
        results.push_back({
            "TC-06",
            "Invalid Marks: Score greater than maximum",
            "Validation error; record not saved",
            actual,
            testPassed
        });
    }

    // ---------------------------------------------------------
    // TC-07: Search Missing ID
    // ---------------------------------------------------------
    {
        std::string missingId = "S_NON_EXISTENT_999";
        Student* found = repo.findById(missingId);
        std::vector<Student> matches = repo.search(missingId);
        bool testPassed = (found == nullptr && matches.empty());
        std::string actual = testPassed ? "No matching record message" : "Found ghost record";
        results.push_back({
            "TC-07",
            "Search Missing ID: ID not present in database",
            "No matching record message",
            actual,
            testPassed
        });
    }

    // ---------------------------------------------------------
    // Print Formatted Report Table
    // ---------------------------------------------------------
    std::cout << std::left
              << std::setw(8)  << "Test"
              << std::setw(32) << "Test Scenario"
              << std::setw(38) << "Expected Result"
              << std::setw(10) << "Status" << "\n";
    std::cout << "----------------------------------------------------------------------------------------------------\n";

    int passedCount = 0;
    for (const auto& r : results) {
        std::cout << std::left
                  << std::setw(8)  << r.testCase
                  << std::setw(32) << r.description
                  << std::setw(38) << r.expected
                  << (r.passed ? "[ PASS ]" : "[ FAIL ]") << "\n";
        if (r.passed) passedCount++;
    }
    std::cout << "----------------------------------------------------------------------------------------------------\n";
    std::cout << "SUMMARY: " << passedCount << " / " << results.size() << " Test Cases PASSED ("
              << (passedCount == (int)results.size() ? "100% SUCCESS" : "FAILURES DETECTED") << ")\n";
    std::cout << "====================================================================================================\n\n";

    return (passedCount == (int)results.size()) ? 0 : 1;
}
