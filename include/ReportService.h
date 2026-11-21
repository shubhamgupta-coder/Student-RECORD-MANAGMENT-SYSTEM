#ifndef REPORT_SERVICE_H
#define REPORT_SERVICE_H

#include <string>
#include <vector>
#include "StudentRepository.h"

struct ReportFilter {
    std::string courseId;
    std::string status;       // "All", "Active", "Inactive"
    std::string department;
};

class ReportService {
private:
    StudentRepository& repo;
    ReportFilter reportFilters;

    std::string calculateGrade(double percentage) const;

public:
    ReportService(StudentRepository& repository);

    void setFilter(const std::string& course, const std::string& status, const std::string& dept);

    // Core methods from specification
    std::string studentList() const;
    std::string marksSummary(const std::string& studentId) const;
    bool exportReport(const std::string& content, const std::string& filename, std::string& errorMessage) const;

    // Academic report card
    std::string generateAcademicReportCard(const std::string& studentId) const;
};

#endif // REPORT_SERVICE_H
