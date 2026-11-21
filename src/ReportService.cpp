#include "../include/ReportService.h"
#include <sstream>
#include <iomanip>
#include <fstream>

ReportService::ReportService(StudentRepository& repository) : repo(repository) {
    reportFilters.courseId = "";
    reportFilters.status = "All";
    reportFilters.department = "";
}

void ReportService::setFilter(const std::string& course, const std::string& status, const std::string& dept) {
    reportFilters.courseId = course;
    reportFilters.status = status;
    reportFilters.department = dept;
}

std::string ReportService::calculateGrade(double percentage) const {
    if (percentage >= 90.0) return "A+ (Outstanding)";
    if (percentage >= 80.0) return "A (Excellent)";
    if (percentage >= 70.0) return "B (Very Good)";
    if (percentage >= 60.0) return "C (Good)";
    if (percentage >= 50.0) return "D (Pass)";
    return "F (Fail)";
}

std::string ReportService::studentList() const {
    std::ostringstream oss;
    std::vector<Student> allStudents = repo.getAllStudents(true);

    oss << "=========================================================================================\n";
    oss << "                          OFFICIAL STUDENT CLASS ROSTER REPORT                           \n";
    oss << "=========================================================================================\n";
    oss << std::left << std::setw(8) << "ID"
        << std::setw(22) << "Full Name"
        << std::setw(12) << "DOB"
        << std::setw(24) << "Email"
        << std::setw(14) << "Phone"
        << std::setw(10) << "Course"
        << std::setw(10) << "Status" << "\n";
    oss << "-----------------------------------------------------------------------------------------\n";

    int count = 0;
    for (const auto& s : allStudents) {
        if (!reportFilters.courseId.empty() && Utils::toUpper(s.getCourseId()) != Utils::toUpper(reportFilters.courseId)) {
            continue;
        }
        if (reportFilters.status == "Active" && !s.isActive()) continue;
        if (reportFilters.status == "Inactive" && s.isActive()) continue;

        oss << std::left << std::setw(8) << s.getStudentId()
            << std::setw(22) << s.getFullName()
            << std::setw(12) << s.getDateOfBirth()
            << std::setw(24) << s.getEmail()
            << std::setw(14) << s.getPhone()
            << std::setw(10) << s.getCourseId()
            << std::setw(10) << s.getStatus() << "\n";
        count++;
    }

    oss << "-----------------------------------------------------------------------------------------\n";
    oss << "Total Records Listed: " << count << "\n";
    oss << "=========================================================================================\n";
    return oss.str();
}

std::string ReportService::marksSummary(const std::string& studentId) const {
    Student* student = repo.findById(studentId);
    if (!student) {
        return "Error: Student with ID '" + studentId + "' not found.";
    }

    std::vector<Mark> marks = repo.getMarksByStudentId(studentId);
    std::ostringstream oss;

    oss << "=========================================================================================\n";
    oss << "                             STUDENT ACADEMIC PERFORMANCE SUMMARY                        \n";
    oss << "=========================================================================================\n";
    oss << "Student ID:    " << student->getStudentId() << "\n";
    oss << "Full Name:     " << student->getFullName() << "\n";
    oss << "Course ID:     " << student->getCourseId() << "\n";
    oss << "Profile Status: " << student->getStatus() << "\n";
    oss << "-----------------------------------------------------------------------------------------\n";

    if (marks.empty()) {
        oss << "No assessment marks recorded for this student.\n";
    } else {
        oss << std::left << std::setw(8) << "Mark ID"
            << std::setw(32) << "Subject Name"
            << std::setw(16) << "Assessment"
            << std::setw(10) << "Score"
            << std::setw(10) << "Max"
            << std::setw(12) << "Percentage" << "\n";
        oss << "-----------------------------------------------------------------------------------------\n";

        double totalScore = 0.0;
        double totalMax = 0.0;

        for (const auto& m : marks) {
            oss << std::left << std::setw(8) << m.getMarkId()
                << std::setw(32) << m.getSubject()
                << std::setw(16) << m.getAssessment()
                << std::setw(10) << m.getScore()
                << std::setw(10) << m.getMaxScore()
                << std::fixed << std::setprecision(2) << m.calculatePercentage() << "%\n";
            totalScore += m.getScore();
            totalMax += m.getMaxScore();
        }

        oss << "-----------------------------------------------------------------------------------------\n";
        double cumulativePct = (totalMax > 0.0) ? (totalScore / totalMax) * 100.0 : 0.0;
        oss << "CUMULATIVE SCORE:      " << totalScore << " / " << totalMax << "\n";
        oss << "CUMULATIVE PERCENTAGE: " << std::fixed << std::setprecision(2) << cumulativePct << "%\n";
        oss << "FINAL LETTER GRADE:    " << calculateGrade(cumulativePct) << "\n";
    }
    oss << "=========================================================================================\n";
    return oss.str();
}

std::string ReportService::generateAcademicReportCard(const std::string& studentId) const {
    return marksSummary(studentId);
}

bool ReportService::exportReport(const std::string& content, const std::string& filename, std::string& errorMessage) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        errorMessage = "Failed to open output file: " + filename;
        return false;
    }
    outFile << content;
    outFile.close();
    return true;
}
