#ifndef STUDENT_REPOSITORY_H
#define STUDENT_REPOSITORY_H

#include <string>
#include <vector>
#include <memory>
#include "Student.h"
#include "Course.h"
#include "User.h"
#include "Enrollment.h"
#include "Mark.h"
#include "Attendance.h"

class StudentRepository {
private:
    std::string studentsFilePath;
    std::string coursesFilePath;
    std::string usersFilePath;
    std::string enrollmentsFilePath;
    std::string marksFilePath;
    std::string attendanceFilePath;

    std::vector<Student> studentsCache;
    std::vector<Course> coursesCache;
    std::vector<User> usersCache;
    std::vector<Enrollment> enrollmentsCache;
    std::vector<Mark> marksCache;
    std::vector<Attendance> attendanceCache;

    void loadStudents();
    void saveStudents();

    void loadCourses();
    void saveCourses();

    void loadUsers();
    void saveUsers();

    void loadEnrollments();
    void saveEnrollments();

    void loadMarks();
    void saveMarks();

    void loadAttendance();
    void saveAttendance();

public:
    StudentRepository(const std::string& dataDir = "data");

    // Student CRUD operations
    bool add(const Student& student, std::string& errorMessage);
    Student* findById(const std::string& studentId);
    std::vector<Student> search(const std::string& keyword);
    bool update(const Student& student, std::string& errorMessage);
    bool deactivate(const std::string& studentId, std::string& errorMessage);
    bool activate(const std::string& studentId, std::string& errorMessage);
    std::vector<Student> getAllStudents(bool includeInactive = true);

    // Course operations
    bool addCourse(const Course& course, std::string& errorMessage);
    Course* findCourseById(const std::string& courseId);
    std::vector<Course> getAllCourses();

    // User operations
    bool addUser(const User& user, std::string& errorMessage);
    User* findUserByUsername(const std::string& username);
    std::vector<User> getAllUsers();

    // Enrollment operations
    bool addEnrollment(const Enrollment& enrollment, std::string& errorMessage);
    std::vector<Enrollment> getEnrollmentsByStudentId(const std::string& studentId);
    std::vector<Enrollment> getAllEnrollments();

    // Marks operations
    bool addMark(const Mark& mark, std::string& errorMessage);
    std::vector<Mark> getMarksByStudentId(const std::string& studentId);
    std::vector<Mark> getAllMarks();

    // Attendance operations
    bool addAttendance(const Attendance& attendance, std::string& errorMessage);
    std::vector<Attendance> getAttendanceByStudentId(const std::string& studentId);
    std::vector<Attendance> getAllAttendance();

    // Seed defaults
    void seedDefaultsIfEmpty();
};

#endif // STUDENT_REPOSITORY_H
