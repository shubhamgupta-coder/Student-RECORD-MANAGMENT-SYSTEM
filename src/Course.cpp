#include "../include/Course.h"

Course::Course() : courseId(""), courseName(""), department(""), durationYears(0) {}

Course::Course(const std::string& cId, const std::string& cName, const std::string& dept, int duration)
    : courseId(cId), courseName(cName), department(dept), durationYears(duration) {}

std::string Course::getCourseId() const { return courseId; }
std::string Course::getCourseName() const { return courseName; }
std::string Course::getDepartment() const { return department; }
int Course::getDurationYears() const { return durationYears; }

bool Course::validateCourse(std::string& errorMessage) const {
    if (Utils::trim(courseId).empty()) {
        errorMessage = "Course ID cannot be empty.";
        return false;
    }
    if (Utils::trim(courseName).empty()) {
        errorMessage = "Course Name cannot be empty.";
        return false;
    }
    if (Utils::trim(department).empty()) {
        errorMessage = "Department cannot be empty.";
        return false;
    }
    if (durationYears <= 0 || durationYears > 6) {
        errorMessage = "Duration must be between 1 and 6 years.";
        return false;
    }
    return true;
}

void Course::updateCourse(const std::string& cName, const std::string& dept, int duration) {
    if (!cName.empty()) courseName = cName;
    if (!dept.empty()) department = dept;
    if (duration > 0) durationYears = duration;
}

std::string Course::serialize() const {
    return courseId + "|" + courseName + "|" + department + "|" + std::to_string(durationYears);
}

Course Course::deserialize(const std::string& line) {
    std::vector<std::string> parts = Utils::split(line, '|');
    if (parts.size() >= 4) {
        int dur = 3;
        try {
            dur = std::stoi(parts[3]);
        } catch (...) {}
        return Course(parts[0], parts[1], parts[2], dur);
    }
    return Course();
}
