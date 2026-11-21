#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <string>
#include <vector>
#include "Utils.h"

class Attendance {
private:
    std::string attendanceId;
    std::string studentId;
    std::string date;      // YYYY-MM-DD
    std::string status;    // "Present", "Absent", "Late"

public:
    Attendance();
    Attendance(const std::string& aId, const std::string& sId, const std::string& dt, const std::string& st);

    // Getters
    std::string getAttendanceId() const;
    std::string getStudentId() const;
    std::string getDate() const;
    std::string getStatus() const;

    // Methods
    bool validate(std::string& errorMessage) const;

    // Serialization
    std::string serialize() const;
    static Attendance deserialize(const std::string& line);
};

#endif // ATTENDANCE_H
