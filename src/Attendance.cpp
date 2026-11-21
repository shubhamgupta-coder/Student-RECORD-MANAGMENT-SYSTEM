#include "../include/Attendance.h"

Attendance::Attendance() : attendanceId(""), studentId(""), date(""), status("Present") {}

Attendance::Attendance(const std::string& aId, const std::string& sId, const std::string& dt, const std::string& st)
    : attendanceId(aId), studentId(sId), date(dt), status(st) {}

std::string Attendance::getAttendanceId() const { return attendanceId; }
std::string Attendance::getStudentId() const { return studentId; }
std::string Attendance::getDate() const { return date; }
std::string Attendance::getStatus() const { return status; }

bool Attendance::validate(std::string& errorMessage) const {
    if (Utils::trim(attendanceId).empty()) {
        errorMessage = "Attendance ID cannot be empty.";
        return false;
    }
    if (Utils::trim(studentId).empty()) {
        errorMessage = "Student ID cannot be empty.";
        return false;
    }
    if (Utils::trim(date).empty()) {
        errorMessage = "Date cannot be empty.";
        return false;
    }
    if (status != "Present" && status != "Absent" && status != "Late") {
        errorMessage = "Status must be 'Present', 'Absent', or 'Late'.";
        return false;
    }
    return true;
}

std::string Attendance::serialize() const {
    return attendanceId + "|" + studentId + "|" + date + "|" + status;
}

Attendance Attendance::deserialize(const std::string& line) {
    std::vector<std::string> parts = Utils::split(line, '|');
    if (parts.size() >= 4) {
        return Attendance(parts[0], parts[1], parts[2], parts[3]);
    }
    return Attendance();
}
