#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include "Utils.h"

class Student {
private:
    std::string studentId;
    std::string fullName;
    std::string dateOfBirth; // YYYY-MM-DD
    std::string email;
    std::string phone;
    std::string address;
    std::string courseId;
    std::string status;      // "Active" or "Inactive"

public:
    Student();
    Student(const std::string& sId, const std::string& name, const std::string& dob,
            const std::string& mail, const std::string& ph, const std::string& addr,
            const std::string& cId, const std::string& st = "Active");

    // Getters
    std::string getStudentId() const;
    std::string getFullName() const;
    std::string getDateOfBirth() const;
    std::string getEmail() const;
    std::string getPhone() const;
    std::string getAddress() const;
    std::string getCourseId() const;
    std::string getStatus() const;
    bool isActive() const;

    // Setters
    void setCourseId(const std::string& cId);
    void setStatus(const std::string& st);

    // Methods from specification
    bool validateDetails(std::string& errorMessage) const;
    void updateProfile(const std::string& name, const std::string& dob,
                       const std::string& mail, const std::string& ph,
                       const std::string& addr, const std::string& cId);
    std::string getSummary() const;

    // Serialization helper
    std::string serialize() const;
    static Student deserialize(const std::string& line);
};

#endif // STUDENT_H
