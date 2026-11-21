#include <iostream>
#include <iomanip>
#include <string>
#include "../include/StudentRepository.h"
#include "../include/User.h"
#include "../include/Student.h"
#include "../include/Course.h"
#include "../include/Utils.h"

void printHeader() {
    std::cout << "\n=================================================================\n";
    std::cout << "                 STUDENT RECORD MANAGEMENT SYSTEM                \n";
    std::cout << "          Prepared by: Shubham Gupta (45) & Swapnil Yadav (46)   \n";
    std::cout << "                      Evaluation 1 - MVP Version                 \n";
    std::cout << "=================================================================\n";
}

void printDivider() {
    std::cout << "-----------------------------------------------------------------\n";
}

int main() {
    StudentRepository repo("data");
    User* currentUser = nullptr;

    while (true) {
        printHeader();

        if (currentUser == nullptr) {
            std::cout << "\n>>> SYSTEM AUTHENTICATION <<<\n";
            std::cout << "1. Login (Admin / Staff)\n";
            std::cout << "2. Quick Test Credentials Info\n";
            std::cout << "0. Exit Application\n";
            std::cout << "Enter Choice: ";

            int authChoice = -1;
            if (!(std::cin >> authChoice)) {
                std::cin.clear();
                std::string dummy;
                std::getline(std::cin, dummy);
                continue;
            }
            std::cin.ignore(1000, '\n');

            if (authChoice == 0) {
                std::cout << "\nExiting System. Thank you!\n";
                break;
            } else if (authChoice == 2) {
                printDivider();
                std::cout << "[INFO] Default System Credentials:\n";
                std::cout << "  Admin: username: admin | password: admin123\n";
                std::cout << "  Staff: username: staff | password: staff123\n";
                printDivider();
                continue;
            } else if (authChoice == 1) {
                std::string username, password;
                std::cout << "Enter Username: ";
                std::getline(std::cin, username);
                std::cout << "Enter Password: ";
                std::getline(std::cin, password);

                username = Utils::trim(username);
                password = Utils::trim(password);

                User* user = repo.findUserByUsername(username);
                if (user && user->login(password)) {
                    currentUser = user;
                    std::cout << "\n[SUCCESS] Login successful! Welcome, " << currentUser->getUsername()
                              << " (" << currentUser->getRole() << ")\n";
                } else {
                    std::cout << "\n[ERROR] Invalid username or password! Access Denied.\n";
                }
            }
        } else {
            // Logged in Dashboard
            std::cout << "\n>>> MAIN DASHBOARD [User: " << currentUser->getUsername()
                      << " | Role: " << currentUser->getRole() << "] <<<\n";
            std::cout << "1. Add New Student Record\n";
            std::cout << "2. View All Student Records\n";
            std::cout << "3. Search Student by ID\n";
            std::cout << "4. View Available Courses\n";
            std::cout << "5. Logout\n";
            std::cout << "0. Exit Application\n";
            std::cout << "Select Operation: ";

            int op = -1;
            if (!(std::cin >> op)) {
                std::cin.clear();
                std::string dummy;
                std::getline(std::cin, dummy);
                continue;
            }
            std::cin.ignore(1000, '\n');

            if (op == 0) {
                std::cout << "\nExiting System. Goodbye!\n";
                break;
            } else if (op == 5) {
                currentUser->logout();
                currentUser = nullptr;
                std::cout << "\n[INFO] You have successfully logged out.\n";
            } else if (op == 1) {
                // Add student
                std::cout << "\n--- ADD NEW STUDENT ---\n";
                std::string sId, name, dob, email, phone, address, courseId;

                std::cout << "Enter Student ID (e.g. S104): ";
                std::getline(std::cin, sId);
                std::cout << "Enter Full Name: ";
                std::getline(std::cin, name);
                std::cout << "Enter Date of Birth (YYYY-MM-DD): ";
                std::getline(std::cin, dob);
                std::cout << "Enter Email Address: ";
                std::getline(std::cin, email);
                std::cout << "Enter Phone Number (10 digits): ";
                std::getline(std::cin, phone);
                std::cout << "Enter Address: ";
                std::getline(std::cin, address);
                std::cout << "Enter Course ID (e.g. CS101, IT102): ";
                std::getline(std::cin, courseId);

                Student newStudent(Utils::trim(sId), Utils::trim(name), Utils::trim(dob),
                                   Utils::trim(email), Utils::trim(phone), Utils::trim(address),
                                   Utils::trim(courseId), "Active");

                std::string validationError;
                if (!newStudent.validateDetails(validationError)) {
                    std::cout << "\n[ERROR] " << validationError << "\n";
                } else {
                    std::string repoError;
                    if (repo.add(newStudent, repoError)) {
                        std::cout << "\n[SUCCESS] Student record saved successfully! Assigned ID: "
                                  << newStudent.getStudentId() << "\n";
                    } else {
                        std::cout << "\n[REJECTED] " << repoError << "\n";
                    }
                }
            } else if (op == 2) {
                // View all students
                std::vector<Student> students = repo.getAllStudents();
                std::cout << "\n--- STUDENT ROSTER (" << students.size() << " Records) ---\n";
                std::cout << std::left << std::setw(8) << "ID"
                          << std::setw(20) << "Full Name"
                          << std::setw(12) << "DOB"
                          << std::setw(22) << "Email"
                          << std::setw(14) << "Phone"
                          << std::setw(10) << "Course"
                          << std::setw(10) << "Status" << "\n";
                printDivider();
                for (const auto& s : students) {
                    std::cout << std::left << std::setw(8) << s.getStudentId()
                              << std::setw(20) << s.getFullName()
                              << std::setw(12) << s.getDateOfBirth()
                              << std::setw(22) << s.getEmail()
                              << std::setw(14) << s.getPhone()
                              << std::setw(10) << s.getCourseId()
                              << std::setw(10) << s.getStatus() << "\n";
                }
                printDivider();
            } else if (op == 3) {
                // Search by ID
                std::cout << "\n--- SEARCH STUDENT BY ID ---\n";
                std::cout << "Enter Student ID to Search: ";
                std::string queryId;
                std::getline(std::cin, queryId);
                queryId = Utils::trim(queryId);

                Student* found = repo.findById(queryId);
                if (found) {
                    printDivider();
                    std::cout << "Student Found:\n";
                    std::cout << "  ID:            " << found->getStudentId() << "\n";
                    std::cout << "  Full Name:     " << found->getFullName() << "\n";
                    std::cout << "  Date of Birth: " << found->getDateOfBirth() << "\n";
                    std::cout << "  Email:         " << found->getEmail() << "\n";
                    std::cout << "  Phone:         " << found->getPhone() << "\n";
                    std::cout << "  Address:       " << found->getAddress() << "\n";
                    std::cout << "  Course:        " << found->getCourseId() << "\n";
                    std::cout << "  Status:        " << found->getStatus() << "\n";
                    printDivider();
                } else {
                    std::cout << "\n[INFO] No matching record found for ID '" << queryId << "'.\n";
                }
            } else if (op == 4) {
                // View courses
                std::vector<Course> courses = repo.getAllCourses();
                std::cout << "\n--- AVAILABLE COURSES (" << courses.size() << ") ---\n";
                std::cout << std::left << std::setw(10) << "Course ID"
                          << std::setw(35) << "Course Name"
                          << std::setw(15) << "Department"
                          << std::setw(10) << "Duration" << "\n";
                printDivider();
                for (const auto& c : courses) {
                    std::cout << std::left << std::setw(10) << c.getCourseId()
                              << std::setw(35) << c.getCourseName()
                              << std::setw(15) << c.getDepartment()
                              << c.getDurationYears() << " Years\n";
                }
                printDivider();
            } else {
                std::cout << "\n[ERROR] Invalid option selected. Please try again.\n";
            }
        }
    }

    return 0;
}
