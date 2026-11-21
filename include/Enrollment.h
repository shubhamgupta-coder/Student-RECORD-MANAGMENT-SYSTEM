#ifndef ENROLLMENT_H
#define ENROLLMENT_H

#include <string>
#include <vector>
#include "Utils.h"

class Enrollment {
private:
    std::string enrollmentId;
    std::string studentId;
    std::string courseId;
    std::string academicYear;
    std::string semester;
    std::string enrollmentDate;

public:
    Enrollment();
    Enrollment(const std::string& eId, const std::string& sId, const std::string& cId,
               const std::string& yr, const std::string& sem, const std::string& date);

    // Getters
    std::string getEnrollmentId() const;
    std::string getStudentId() const;
    std::string getCourseId() const;
    std::string getAcademicYear() const;
    std::string getSemester() const;
    std::string getEnrollmentDate() const;

    // Methods from specification
    void enrollStudent(const std::string& sId, const std::string& cId,
                       const std::string& yr, const std::string& sem, const std::string& date);
    void changeCourse(const std::string& newCourseId);
    std::string getEnrollmentDetails() const;

    // Serialization helper
    std::string serialize() const;
    static Enrollment deserialize(const std::string& line);
};

#endif // ENROLLMENT_H
