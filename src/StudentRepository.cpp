#include "../include/StudentRepository.h"
#include <fstream>
#include <iostream>
#include <algorithm>

StudentRepository::StudentRepository(const std::string& dataDir)
    : studentsFilePath(dataDir + "/students.txt"),
      coursesFilePath(dataDir + "/courses.txt"),
      usersFilePath(dataDir + "/users.txt"),
      enrollmentsFilePath(dataDir + "/enrollments.txt"),
      marksFilePath(dataDir + "/marks.txt"),
      attendanceFilePath(dataDir + "/attendance.txt") {
    loadUsers();
    loadCourses();
    loadStudents();
    loadEnrollments();
    loadMarks();
    loadAttendance();
    seedDefaultsIfEmpty();
}

void StudentRepository::loadUsers() {
    usersCache.clear();
    std::ifstream inFile(usersFilePath);
    if (!inFile.is_open()) return;
    std::string line;
    while (std::getline(inFile, line)) {
        line = Utils::trim(line);
        if (!line.empty() && line[0] != '#') {
            usersCache.push_back(User::deserialize(line));
        }
    }
    inFile.close();
}

void StudentRepository::saveUsers() {
    std::ofstream outFile(usersFilePath);
    if (!outFile.is_open()) return;
    for (const auto& u : usersCache) {
        outFile << u.serialize() << "\n";
    }
    outFile.close();
}

void StudentRepository::loadCourses() {
    coursesCache.clear();
    std::ifstream inFile(coursesFilePath);
    if (!inFile.is_open()) return;
    std::string line;
    while (std::getline(inFile, line)) {
        line = Utils::trim(line);
        if (!line.empty() && line[0] != '#') {
            coursesCache.push_back(Course::deserialize(line));
        }
    }
    inFile.close();
}

void StudentRepository::saveCourses() {
    std::ofstream outFile(coursesFilePath);
    if (!outFile.is_open()) return;
    for (const auto& c : coursesCache) {
        outFile << c.serialize() << "\n";
    }
    outFile.close();
}

void StudentRepository::loadStudents() {
    studentsCache.clear();
    std::ifstream inFile(studentsFilePath);
    if (!inFile.is_open()) return;
    std::string line;
    while (std::getline(inFile, line)) {
        line = Utils::trim(line);
        if (!line.empty() && line[0] != '#') {
            studentsCache.push_back(Student::deserialize(line));
        }
    }
    inFile.close();
}

void StudentRepository::saveStudents() {
    std::ofstream outFile(studentsFilePath);
    if (!outFile.is_open()) return;
    for (const auto& s : studentsCache) {
        outFile << s.serialize() << "\n";
    }
    outFile.close();
}

void StudentRepository::loadEnrollments() {
    enrollmentsCache.clear();
    std::ifstream inFile(enrollmentsFilePath);
    if (!inFile.is_open()) return;
    std::string line;
    while (std::getline(inFile, line)) {
        line = Utils::trim(line);
        if (!line.empty() && line[0] != '#') {
            enrollmentsCache.push_back(Enrollment::deserialize(line));
        }
    }
    inFile.close();
}

void StudentRepository::saveEnrollments() {
    std::ofstream outFile(enrollmentsFilePath);
    if (!outFile.is_open()) return;
    for (const auto& e : enrollmentsCache) {
        outFile << e.serialize() << "\n";
    }
    outFile.close();
}

void StudentRepository::loadMarks() {
    marksCache.clear();
    std::ifstream inFile(marksFilePath);
    if (!inFile.is_open()) return;
    std::string line;
    while (std::getline(inFile, line)) {
        line = Utils::trim(line);
        if (!line.empty() && line[0] != '#') {
            marksCache.push_back(Mark::deserialize(line));
        }
    }
    inFile.close();
}

void StudentRepository::saveMarks() {
    std::ofstream outFile(marksFilePath);
    if (!outFile.is_open()) return;
    for (const auto& m : marksCache) {
        outFile << m.serialize() << "\n";
    }
    outFile.close();
}

void StudentRepository::loadAttendance() {
    attendanceCache.clear();
    std::ifstream inFile(attendanceFilePath);
    if (!inFile.is_open()) return;
    std::string line;
    while (std::getline(inFile, line)) {
        line = Utils::trim(line);
        if (!line.empty() && line[0] != '#') {
            attendanceCache.push_back(Attendance::deserialize(line));
        }
    }
    inFile.close();
}

void StudentRepository::saveAttendance() {
    std::ofstream outFile(attendanceFilePath);
    if (!outFile.is_open()) return;
    for (const auto& a : attendanceCache) {
        outFile << a.serialize() << "\n";
    }
    outFile.close();
}

void StudentRepository::seedDefaultsIfEmpty() {
    // Seed default users if empty
    if (usersCache.empty()) {
        usersCache.push_back(User("U001", "admin", Utils::hashPassword("admin123"), "Admin", true));
        usersCache.push_back(User("U002", "staff", Utils::hashPassword("staff123"), "Staff", true));
        saveUsers();
    }

    // Seed default courses if empty
    if (coursesCache.empty()) {
        coursesCache.push_back(Course("CS101", "Computer Science & Engineering", "CSE", 4));
        coursesCache.push_back(Course("IT102", "Information Technology", "IT", 4));
        coursesCache.push_back(Course("EC103", "Electronics & Communication", "ECE", 4));
        coursesCache.push_back(Course("ME104", "Mechanical Engineering", "MECH", 4));
        saveCourses();
    }

    // Seed default students if empty
    if (studentsCache.empty()) {
        studentsCache.push_back(Student("S101", "Shubham Gupta", "2003-05-14", "shubham@univ.edu", "9876543210", "Block A, Campus Hostel", "CS101", "Active"));
        studentsCache.push_back(Student("S102", "Swapnil Yadav", "2003-08-22", "swapnil@univ.edu", "9876543211", "Block B, Campus Hostel", "CS101", "Active"));
        studentsCache.push_back(Student("S103", "Aarav Sharma", "2003-01-10", "aarav@univ.edu", "9876543212", "Green Park, City", "IT102", "Active"));
        saveStudents();
    }

    // Seed default enrollments if empty
    if (enrollmentsCache.empty()) {
        enrollmentsCache.push_back(Enrollment("E001", "S101", "CS101", "2024-2025", "Semester 5", "2024-07-15"));
        enrollmentsCache.push_back(Enrollment("E002", "S102", "CS101", "2024-2025", "Semester 5", "2024-07-15"));
        enrollmentsCache.push_back(Enrollment("E003", "S103", "IT102", "2024-2025", "Semester 5", "2024-07-16"));
        saveEnrollments();
    }

    // Seed default marks if empty
    if (marksCache.empty()) {
        marksCache.push_back(Mark("M001", "S101", "Data Structures & Algorithms", "Final Exam", 92.5, 100.0));
        marksCache.push_back(Mark("M002", "S101", "Database Management Systems", "Final Exam", 88.0, 100.0));
        marksCache.push_back(Mark("M003", "S101", "Computer Networks", "Mid-Term", 46.0, 50.0));

        marksCache.push_back(Mark("M004", "S102", "Data Structures & Algorithms", "Final Exam", 89.0, 100.0));
        marksCache.push_back(Mark("M005", "S102", "Database Management Systems", "Final Exam", 91.5, 100.0));
        marksCache.push_back(Mark("M006", "S102", "Operating Systems", "Mid-Term", 44.0, 50.0));

        marksCache.push_back(Mark("M007", "S103", "Web Development", "Final Exam", 85.0, 100.0));
        marksCache.push_back(Mark("M008", "S103", "Software Engineering", "Final Exam", 82.0, 100.0));
        saveMarks();
    }

    // Seed default attendance if empty
    if (attendanceCache.empty()) {
        attendanceCache.push_back(Attendance("A001", "S101", "2024-11-01", "Present"));
        attendanceCache.push_back(Attendance("A002", "S101", "2024-11-02", "Present"));
        attendanceCache.push_back(Attendance("A003", "S101", "2024-11-03", "Present"));
        attendanceCache.push_back(Attendance("A004", "S102", "2024-11-01", "Present"));
        attendanceCache.push_back(Attendance("A005", "S102", "2024-11-02", "Absent"));
        attendanceCache.push_back(Attendance("A006", "S102", "2024-11-03", "Present"));
        attendanceCache.push_back(Attendance("A007", "S103", "2024-11-01", "Present"));
        attendanceCache.push_back(Attendance("A008", "S103", "2024-11-02", "Present"));
        saveAttendance();
    }
}

bool StudentRepository::add(const Student& student, std::string& errorMessage) {
    // TC-04: System blocks duplicate ID and explains why
    for (const auto& s : studentsCache) {
        if (Utils::toUpper(s.getStudentId()) == Utils::toUpper(student.getStudentId())) {
            errorMessage = "Conflict Error: Student ID '" + student.getStudentId() + "' already exists in the system.";
            return false;
        }
    }
    studentsCache.push_back(student);
    saveStudents();
    return true;
}

Student* StudentRepository::findById(const std::string& studentId) {
    std::string upperTarget = Utils::toUpper(Utils::trim(studentId));
    for (auto& s : studentsCache) {
        if (Utils::toUpper(s.getStudentId()) == upperTarget) {
            return &s;
        }
    }
    return nullptr;
}

std::vector<Student> StudentRepository::search(const std::string& keyword) {
    std::vector<Student> results;
    std::string lowerKeyword = keyword;
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);

    for (const auto& s : studentsCache) {
        std::string sId = s.getStudentId();
        std::string sName = s.getFullName();
        std::string sCourse = s.getCourseId();

        std::transform(sId.begin(), sId.end(), sId.begin(), ::tolower);
        std::transform(sName.begin(), sName.end(), sName.begin(), ::tolower);
        std::transform(sCourse.begin(), sCourse.end(), sCourse.begin(), ::tolower);

        if (sId.find(lowerKeyword) != std::string::npos ||
            sName.find(lowerKeyword) != std::string::npos ||
            sCourse.find(lowerKeyword) != std::string::npos) {
            results.push_back(s);
        }
    }
    return results;
}

bool StudentRepository::update(const Student& student, std::string& errorMessage) {
    for (auto& s : studentsCache) {
        if (Utils::toUpper(s.getStudentId()) == Utils::toUpper(student.getStudentId())) {
            s = student;
            saveStudents();
            return true;
        }
    }
    errorMessage = "Update Error: Student with ID '" + student.getStudentId() + "' not found.";
    return false;
}

bool StudentRepository::deactivate(const std::string& studentId, std::string& errorMessage) {
    Student* s = findById(studentId);
    if (!s) {
        errorMessage = "Error: Student with ID '" + studentId + "' not found.";
        return false;
    }
    s->setStatus("Inactive");
    saveStudents();
    return true;
}

bool StudentRepository::activate(const std::string& studentId, std::string& errorMessage) {
    Student* s = findById(studentId);
    if (!s) {
        errorMessage = "Error: Student with ID '" + studentId + "' not found.";
        return false;
    }
    s->setStatus("Active");
    saveStudents();
    return true;
}

bool StudentRepository::remove(const std::string& studentId, std::string& errorMessage) {
    std::string upperTarget = Utils::toUpper(Utils::trim(studentId));
    auto it = std::remove_if(studentsCache.begin(), studentsCache.end(), [&](const Student& s) {
        return Utils::toUpper(s.getStudentId()) == upperTarget;
    });

    if (it != studentsCache.end()) {
        studentsCache.erase(it, studentsCache.end());
        saveStudents();
        return true;
    }
    errorMessage = "Delete Error: Student ID '" + studentId + "' not found.";
    return false;
}

std::vector<Student> StudentRepository::getAllStudents(bool includeInactive) {
    if (includeInactive) {
        return studentsCache;
    }
    std::vector<Student> activeList;
    for (const auto& s : studentsCache) {
        if (s.isActive()) activeList.push_back(s);
    }
    return activeList;
}

bool StudentRepository::addCourse(const Course& course, std::string& errorMessage) {
    for (const auto& c : coursesCache) {
        if (Utils::toUpper(c.getCourseId()) == Utils::toUpper(course.getCourseId())) {
            errorMessage = "Course ID '" + course.getCourseId() + "' already exists.";
            return false;
        }
    }
    coursesCache.push_back(course);
    saveCourses();
    return true;
}

Course* StudentRepository::findCourseById(const std::string& courseId) {
    std::string upperTarget = Utils::toUpper(Utils::trim(courseId));
    for (auto& c : coursesCache) {
        if (Utils::toUpper(c.getCourseId()) == upperTarget) {
            return &c;
        }
    }
    return nullptr;
}

std::vector<Course> StudentRepository::getAllCourses() {
    return coursesCache;
}

bool StudentRepository::addUser(const User& user, std::string& errorMessage) {
    for (const auto& u : usersCache) {
        if (u.getUsername() == user.getUsername()) {
            errorMessage = "Username '" + user.getUsername() + "' already exists.";
            return false;
        }
    }
    usersCache.push_back(user);
    saveUsers();
    return true;
}

User* StudentRepository::findUserByUsername(const std::string& username) {
    for (auto& u : usersCache) {
        if (u.getUsername() == username) {
            return &u;
        }
    }
    return nullptr;
}

std::vector<User> StudentRepository::getAllUsers() {
    return usersCache;
}

bool StudentRepository::addEnrollment(const Enrollment& enrollment, std::string& errorMessage) {
    Student* student = findById(enrollment.getStudentId());
    if (!student) {
        errorMessage = "Foreign Key Error: Student ID '" + enrollment.getStudentId() + "' does not exist.";
        return false;
    }

    Course* course = findCourseById(enrollment.getCourseId());
    if (!course) {
        errorMessage = "Foreign Key Error: Course ID '" + enrollment.getCourseId() + "' does not exist.";
        return false;
    }

    enrollmentsCache.push_back(enrollment);
    saveEnrollments();
    return true;
}

std::vector<Enrollment> StudentRepository::getEnrollmentsByStudentId(const std::string& studentId) {
    std::vector<Enrollment> results;
    std::string upperId = Utils::toUpper(Utils::trim(studentId));
    for (const auto& e : enrollmentsCache) {
        if (Utils::toUpper(e.getStudentId()) == upperId) {
            results.push_back(e);
        }
    }
    return results;
}

std::vector<Enrollment> StudentRepository::getAllEnrollments() {
    return enrollmentsCache;
}

bool StudentRepository::addMark(const Mark& mark, std::string& errorMessage) {
    if (!mark.validateScore(errorMessage)) {
        return false;
    }

    Student* student = findById(mark.getStudentId());
    if (!student) {
        errorMessage = "Foreign Key Error: Cannot enter mark. Student ID '" + mark.getStudentId() + "' does not exist.";
        return false;
    }

    marksCache.push_back(mark);
    saveMarks();
    return true;
}

std::vector<Mark> StudentRepository::getMarksByStudentId(const std::string& studentId) {
    std::vector<Mark> results;
    std::string upperId = Utils::toUpper(Utils::trim(studentId));
    for (const auto& m : marksCache) {
        if (Utils::toUpper(m.getStudentId()) == upperId) {
            results.push_back(m);
        }
    }
    return results;
}

std::vector<Mark> StudentRepository::getAllMarks() {
    return marksCache;
}

bool StudentRepository::addAttendance(const Attendance& attendance, std::string& errorMessage) {
    if (!attendance.validate(errorMessage)) {
        return false;
    }

    Student* student = findById(attendance.getStudentId());
    if (!student) {
        errorMessage = "Foreign Key Error: Student ID '" + attendance.getStudentId() + "' does not exist.";
        return false;
    }

    attendanceCache.push_back(attendance);
    saveAttendance();
    return true;
}

std::vector<Attendance> StudentRepository::getAttendanceByStudentId(const std::string& studentId) {
    std::vector<Attendance> results;
    std::string upperId = Utils::toUpper(Utils::trim(studentId));
    for (const auto& a : attendanceCache) {
        if (Utils::toUpper(a.getStudentId()) == upperId) {
            results.push_back(a);
        }
    }
    return results;
}

std::vector<Attendance> StudentRepository::getAllAttendance() {
    return attendanceCache;
}
