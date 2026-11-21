#ifndef STUDENT_REPOSITORY_H
#define STUDENT_REPOSITORY_H

#include <string>
#include <vector>
#include <memory>
#include "Student.h"
#include "Course.h"
#include "User.h"

class StudentRepository {
private:
    std::string studentsFilePath;
    std::string coursesFilePath;
    std::string usersFilePath;

    std::vector<Student> studentsCache;
    std::vector<Course> coursesCache;
    std::vector<User> usersCache;

    void loadStudents();
    void saveStudents();

    void loadCourses();
    void saveCourses();

    void loadUsers();
    void saveUsers();

public:
    StudentRepository(const std::string& dataDir = "data");

    // Student CRUD operations
    bool add(const Student& student, std::string& errorMessage);
    Student* findById(const std::string& studentId);
    std::vector<Student> search(const std::string& keyword);
    bool update(const Student& student, std::string& errorMessage);
    bool deactivate(const std::string& studentId, std::string& errorMessage);
    std::vector<Student> getAllStudents(bool includeInactive = true);

    // Course operations
    bool addCourse(const Course& course, std::string& errorMessage);
    Course* findCourseById(const std::string& courseId);
    std::vector<Course> getAllCourses();

    // User operations
    bool addUser(const User& user, std::string& errorMessage);
    User* findUserByUsername(const std::string& username);
    std::vector<User> getAllUsers();

    // Seed defaults
    void seedDefaultsIfEmpty();
};

#endif // STUDENT_REPOSITORY_H
