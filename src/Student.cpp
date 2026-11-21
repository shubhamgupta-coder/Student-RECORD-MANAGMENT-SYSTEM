#include "../include/Student.h"
#include <regex>
#include <sstream>

Student::Student()
    : studentId(""), fullName(""), dateOfBirth(""), email(""), phone(""), address(""), courseId(""), status("Active") {}

Student::Student(const std::string& sId, const std::string& name, const std::string& dob,
                 const std::string& mail, const std::string& ph, const std::string& addr,
                 const std::string& cId, const std::string& st)
    : studentId(sId), fullName(name), dateOfBirth(dob), email(mail), phone(ph), address(addr), courseId(cId), status(st) {}

std::string Student::getStudentId() const { return studentId; }
std::string Student::getFullName() const { return fullName; }
std::string Student::getDateOfBirth() const { return dateOfBirth; }
std::string Student::getEmail() const { return email; }
std::string Student::getPhone() const { return phone; }
std::string Student::getAddress() const { return address; }
std::string Student::getCourseId() const { return courseId; }
std::string Student::getStatus() const { return status; }
bool Student::isActive() const { return status == "Active"; }

void Student::setCourseId(const std::string& cId) { courseId = cId; }
void Student::setStatus(const std::string& st) { status = st; }

bool Student::validateDetails(std::string& errorMessage) const {
    if (Utils::trim(studentId).empty()) {
        errorMessage = "Validation Error: Student ID cannot be empty.";
        return false;
    }
    if (Utils::trim(fullName).empty()) {
        errorMessage = "Validation Error: Full Name cannot be empty.";
        return false;
    }
    // Validate email format if provided
    if (!email.empty()) {
        if (email.find('@') == std::string::npos || email.find('.') == std::string::npos) {
            errorMessage = "Validation Error: Invalid email format (must contain '@' and domain).";
            return false;
        }
    }
    // Validate phone number format if provided (digits only, 7 to 15 length)
    if (!phone.empty()) {
        std::string trimmedPhone = Utils::trim(phone);
        for (char c : trimmedPhone) {
            if (!std::isdigit(static_cast<unsigned char>(c)) && c != '+' && c != '-' && c != ' ') {
                errorMessage = "Validation Error: Phone must contain only numbers, dashes or plus signs.";
                return false;
            }
        }
        int digitCount = 0;
        for (char c : trimmedPhone) {
            if (std::isdigit(static_cast<unsigned char>(c))) digitCount++;
        }
        if (digitCount < 7 || digitCount > 15) {
            errorMessage = "Validation Error: Phone number must contain between 7 and 15 digits.";
            return false;
        }
    }
    return true;
}

void Student::updateProfile(const std::string& name, const std::string& dob,
                           const std::string& mail, const std::string& ph,
                           const std::string& addr, const std::string& cId) {
    if (!name.empty()) fullName = name;
    if (!dob.empty()) dateOfBirth = dob;
    if (!mail.empty()) email = mail;
    if (!ph.empty()) phone = ph;
    if (!addr.empty()) address = addr;
    if (!cId.empty()) courseId = cId;
}

std::string Student::getSummary() const {
    std::ostringstream oss;
    oss << "ID: " << studentId << " | Name: " << fullName
        << " | Course: " << courseId << " | Status: " << status;
    return oss.str();
}

std::string Student::serialize() const {
    return studentId + "|" + fullName + "|" + dateOfBirth + "|" +
           email + "|" + phone + "|" + address + "|" + courseId + "|" + status;
}

Student Student::deserialize(const std::string& line) {
    std::vector<std::string> parts = Utils::split(line, '|');
    if (parts.size() >= 8) {
        return Student(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[6], parts[7]);
    } else if (parts.size() == 7) {
        return Student(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[6], "Active");
    }
    return Student();
}
