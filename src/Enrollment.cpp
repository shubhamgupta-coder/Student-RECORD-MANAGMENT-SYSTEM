#include "../include/Enrollment.h"
#include <sstream>

Enrollment::Enrollment()
    : enrollmentId(""), studentId(""), courseId(""), academicYear(""), semester(""), enrollmentDate("") {}

Enrollment::Enrollment(const std::string& eId, const std::string& sId, const std::string& cId,
                       const std::string& yr, const std::string& sem, const std::string& date)
    : enrollmentId(eId), studentId(sId), courseId(cId), academicYear(yr), semester(sem), enrollmentDate(date) {}

std::string Enrollment::getEnrollmentId() const { return enrollmentId; }
std::string Enrollment::getStudentId() const { return studentId; }
std::string Enrollment::getCourseId() const { return courseId; }
std::string Enrollment::getAcademicYear() const { return academicYear; }
std::string Enrollment::getSemester() const { return semester; }
std::string Enrollment::getEnrollmentDate() const { return enrollmentDate; }

void Enrollment::enrollStudent(const std::string& sId, const std::string& cId,
                               const std::string& yr, const std::string& sem, const std::string& date) {
    studentId = sId;
    courseId = cId;
    academicYear = yr;
    semester = sem;
    enrollmentDate = date;
}

void Enrollment::changeCourse(const std::string& newCourseId) {
    courseId = newCourseId;
}

std::string Enrollment::getEnrollmentDetails() const {
    std::ostringstream oss;
    oss << "Enrollment ID: " << enrollmentId
        << " | Student: " << studentId
        << " | Course: " << courseId
        << " | Year: " << academicYear
        << " | Sem: " << semester
        << " | Date: " << enrollmentDate;
    return oss.str();
}

std::string Enrollment::serialize() const {
    return enrollmentId + "|" + studentId + "|" + courseId + "|" +
           academicYear + "|" + semester + "|" + enrollmentDate;
}

Enrollment Enrollment::deserialize(const std::string& line) {
    std::vector<std::string> parts = Utils::split(line, '|');
    if (parts.size() >= 6) {
        return Enrollment(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5]);
    }
    return Enrollment();
}
