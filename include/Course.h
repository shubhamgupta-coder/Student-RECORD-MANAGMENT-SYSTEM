#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include "Utils.h"

class Course {
private:
    std::string courseId;
    std::string courseName;
    std::string department;
    int durationYears;

public:
    Course();
    Course(const std::string& cId, const std::string& cName, const std::string& dept, int duration);

    // Getters
    std::string getCourseId() const;
    std::string getCourseName() const;
    std::string getDepartment() const;
    int getDurationYears() const;

    // Methods
    bool validateCourse(std::string& errorMessage) const;
    void updateCourse(const std::string& cName, const std::string& dept, int duration);

    // Serialization helper
    std::string serialize() const;
    static Course deserialize(const std::string& line);
};

#endif // COURSE_H
