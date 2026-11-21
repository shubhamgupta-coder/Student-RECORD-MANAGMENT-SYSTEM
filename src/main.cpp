#include <iostream>
#include <iomanip>
#include <string>
#include "../include/StudentRepository.h"
#include "../include/User.h"
#include "../include/Student.h"
#include "../include/Course.h"
#include "../include/Enrollment.h"
#include "../include/Mark.h"
#include "../include/Utils.h"

void printHeader() {
    std::cout << "\n=================================================================\n";
    std::cout << "                 STUDENT RECORD MANAGEMENT SYSTEM                \n";
    std::cout << "          Prepared by: Shubham Gupta (45) & Swapnil Yadav (46)   \n";
    std::cout << "                   Evaluation 2 - Enhanced Version               \n";
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
            std::cout << "\n>>> DASHBOARD [User: " << currentUser->getUsername()
                      << " | Role: " << currentUser->getRole() << "] <<<\n";
            std::cout << "1. Student Management (Add, View, Update)\n";
            std::cout << "2. Advanced Multi-Criteria Search\n";
            std::cout << "3. Course Management (Add, View Courses)\n";
            std::cout << "4. Enrollment Module (Enroll Student to Course/Term)\n";
            std::cout << "5. Marks Module (Record Marks & View Percentages)\n";
            std::cout << "6. Logout\n";
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
            } else if (op == 6) {
                currentUser->logout();
                currentUser = nullptr;
                std::cout << "\n[INFO] You have successfully logged out.\n";
            } else if (op == 1) {
                // Student Management Sub-menu
                std::cout << "\n--- STUDENT MANAGEMENT ---\n";
                std::cout << "1. Add New Student Record\n";
                std::cout << "2. View All Student Records\n";
                std::cout << "3. Update Student Profile (Phone, Address, Course)\n";
                std::cout << "Select Option: ";
                int sOp = -1;
                std::cin >> sOp;
                std::cin.ignore(1000, '\n');

                if (sOp == 1) {
                    std::cout << "\n>>> ADD NEW STUDENT <<<\n";
                    std::string sId, name, dob, email, phone, address, courseId;

                    std::cout << "Enter Student ID (e.g. S104): ";
                    std::getline(std::cin, sId);
                    std::cout << "Enter Full Name: ";
                    std::getline(std::cin, name);
                    std::cout << "Enter Date of Birth (YYYY-MM-DD): ";
                    std::getline(std::cin, dob);
                    std::cout << "Enter Email Address: ";
                    std::getline(std::cin, email);
                    std::cout << "Enter Phone Number (7-15 digits): ";
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
                } else if (sOp == 2) {
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
                } else if (sOp == 3) {
                    // TC-05: Update profile (phone or address)
                    std::cout << "\n>>> UPDATE STUDENT PROFILE <<<\n";
                    std::cout << "Enter Student ID to update: ";
                    std::string targetId;
                    std::getline(std::cin, targetId);
                    Student* existing = repo.findById(targetId);
                    if (!existing) {
                        std::cout << "\n[ERROR] Student ID '" << targetId << "' not found.\n";
                    } else {
                        std::cout << "Editing profile for: " << existing->getFullName() << " (" << existing->getStudentId() << ")\n";
                        std::cout << "Current Phone: " << existing->getPhone() << " -> Enter new phone (or press Enter to keep): ";
                        std::string newPhone;
                        std::getline(std::cin, newPhone);
                        if (!Utils::trim(newPhone).empty()) {
                            existing->updateProfile("", "", "", Utils::trim(newPhone), "", "");
                        }

                        std::cout << "Current Address: " << existing->getAddress() << " -> Enter new address (or press Enter to keep): ";
                        std::string newAddr;
                        std::getline(std::cin, newAddr);
                        if (!Utils::trim(newAddr).empty()) {
                            existing->updateProfile("", "", "", "", Utils::trim(newAddr), "");
                        }

                        std::string updateErr;
                        if (repo.update(*existing, updateErr)) {
                            std::cout << "\n[SUCCESS] Profile updated successfully!\n";
                            std::cout << "Updated Details:\n";
                            std::cout << "  Phone:   " << existing->getPhone() << "\n";
                            std::cout << "  Address: " << existing->getAddress() << "\n";
                        } else {
                            std::cout << "\n[ERROR] " << updateErr << "\n";
                        }
                    }
                }
            } else if (op == 2) {
                // Multi-criteria search
                std::cout << "\n--- ADVANCED MULTI-CRITERIA SEARCH ---\n";
                std::cout << "Enter search keyword (Student ID, Name, or Course): ";
                std::string keyword;
                std::getline(std::cin, keyword);
                keyword = Utils::trim(keyword);

                std::vector<Student> matches = repo.search(keyword);
                if (matches.empty()) {
                    // TC-07 Search missing ID or term
                    std::cout << "\n[INFO] No matching record message: No records found matching '" << keyword << "'.\n";
                } else {
                    std::cout << "\nFound " << matches.size() << " Matching Record(s):\n";
                    printDivider();
                    for (const auto& s : matches) {
                        std::cout << "ID: " << s.getStudentId()
                                  << " | Name: " << s.getFullName()
                                  << " | Course: " << s.getCourseId()
                                  << " | Phone: " << s.getPhone()
                                  << " | Status: " << s.getStatus() << "\n";
                    }
                    printDivider();
                }
            } else if (op == 3) {
                // Course Management
                std::cout << "\n--- COURSE MANAGEMENT ---\n";
                std::cout << "1. Add New Course\n";
                std::cout << "2. View All Courses\n";
                std::cout << "Select Option: ";
                int cOp = -1;
                std::cin >> cOp;
                std::cin.ignore(1000, '\n');

                if (cOp == 1) {
                    std::string cId, cName, dept;
                    int dur = 0;
                    std::cout << "Enter Course ID (e.g. CS105): ";
                    std::getline(std::cin, cId);
                    std::cout << "Enter Course Name: ";
                    std::getline(std::cin, cName);
                    std::cout << "Enter Department: ";
                    std::getline(std::cin, dept);
                    std::cout << "Enter Duration in Years (1-6): ";
                    std::cin >> dur;
                    std::cin.ignore(1000, '\n');

                    Course newCourse(Utils::trim(cId), Utils::trim(cName), Utils::trim(dept), dur);
                    std::string cErr;
                    if (!newCourse.validateCourse(cErr)) {
                        std::cout << "\n[ERROR] " << cErr << "\n";
                    } else if (repo.addCourse(newCourse, cErr)) {
                        std::cout << "\n[SUCCESS] Course " << newCourse.getCourseId() << " added successfully!\n";
                    } else {
                        std::cout << "\n[ERROR] " << cErr << "\n";
                    }
                } else if (cOp == 2) {
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
                }
            } else if (op == 4) {
                // Enrollment Module
                std::cout << "\n--- ENROLLMENT MODULE ---\n";
                std::cout << "1. Enroll Student to Course / Academic Term\n";
                std::cout << "2. View All Enrollments\n";
                std::cout << "3. View Enrollments by Student ID\n";
                std::cout << "Select Option: ";
                int eOp = -1;
                std::cin >> eOp;
                std::cin.ignore(1000, '\n');

                if (eOp == 1) {
                    std::string eId, sId, cId, year, sem, date;
                    std::cout << "Enter Enrollment ID (e.g. E004): ";
                    std::getline(std::cin, eId);
                    std::cout << "Enter Student ID: ";
                    std::getline(std::cin, sId);
                    std::cout << "Enter Course ID: ";
                    std::getline(std::cin, cId);
                    std::cout << "Enter Academic Year (e.g. 2024-2025): ";
                    std::getline(std::cin, year);
                    std::cout << "Enter Semester (e.g. Semester 5): ";
                    std::getline(std::cin, sem);
                    std::cout << "Enter Enrollment Date (YYYY-MM-DD): ";
                    std::getline(std::cin, date);

                    Enrollment newEnr(Utils::trim(eId), Utils::trim(sId), Utils::trim(cId),
                                      Utils::trim(year), Utils::trim(sem), Utils::trim(date));
                    std::string eErr;
                    if (repo.addEnrollment(newEnr, eErr)) {
                        std::cout << "\n[SUCCESS] Student " << sId << " successfully enrolled in " << cId << "!\n";
                    } else {
                        std::cout << "\n[ERROR] " << eErr << "\n";
                    }
                } else if (eOp == 2) {
                    std::vector<Enrollment> enrollments = repo.getAllEnrollments();
                    std::cout << "\n--- ALL ENROLLMENT RECORDS (" << enrollments.size() << ") ---\n";
                    std::cout << std::left << std::setw(12) << "Enroll ID"
                              << std::setw(12) << "Student ID"
                              << std::setw(12) << "Course ID"
                              << std::setw(15) << "Academic Year"
                              << std::setw(15) << "Semester"
                              << std::setw(12) << "Date" << "\n";
                    printDivider();
                    for (const auto& e : enrollments) {
                        std::cout << std::left << std::setw(12) << e.getEnrollmentId()
                                  << std::setw(12) << e.getStudentId()
                                  << std::setw(12) << e.getCourseId()
                                  << std::setw(15) << e.getAcademicYear()
                                  << std::setw(15) << e.getSemester()
                                  << std::setw(12) << e.getEnrollmentDate() << "\n";
                    }
                    printDivider();
                } else if (eOp == 3) {
                    std::cout << "Enter Student ID: ";
                    std::string sId;
                    std::getline(std::cin, sId);
                    std::vector<Enrollment> list = repo.getEnrollmentsByStudentId(sId);
                    if (list.empty()) {
                        std::cout << "\n[INFO] No enrollment records found for Student ID '" << sId << "'.\n";
                    } else {
                        std::cout << "\nEnrollments for Student " << sId << ":\n";
                        for (const auto& e : list) {
                            std::cout << "  - " << e.getEnrollmentDetails() << "\n";
                        }
                    }
                }
            } else if (op == 5) {
                // Marks Module
                std::cout << "\n--- MARKS MANAGEMENT & ASSESSMENT ---\n";
                std::cout << "1. Record New Assessment Mark\n";
                std::cout << "2. View Marks for a Student\n";
                std::cout << "3. View All Assessment Records\n";
                std::cout << "Select Option: ";
                int mOp = -1;
                std::cin >> mOp;
                std::cin.ignore(1000, '\n');

                if (mOp == 1) {
                    std::string mId, sId, subject, assess;
                    double score = 0.0, maxScore = 100.0;

                    std::cout << "Enter Mark ID (e.g. M009): ";
                    std::getline(std::cin, mId);
                    std::cout << "Enter Student ID: ";
                    std::getline(std::cin, sId);
                    std::cout << "Enter Subject Name: ";
                    std::getline(std::cin, subject);
                    std::cout << "Enter Assessment Type (Mid-Term, Final Exam, Quiz): ";
                    std::getline(std::cin, assess);
                    std::cout << "Enter Max Score Allowed (e.g. 100 or 50): ";
                    std::cin >> maxScore;
                    std::cout << "Enter Score Obtained: ";
                    std::cin >> score;
                    std::cin.ignore(1000, '\n');

                    Mark newMark(Utils::trim(mId), Utils::trim(sId), Utils::trim(subject),
                                 Utils::trim(assess), score, maxScore);

                    std::string mErr;
                    // TC-06 Validation
                    if (!newMark.validateScore(mErr)) {
                        std::cout << "\n[VALIDATION ERROR] " << mErr << " Record not saved.\n";
                    } else if (repo.addMark(newMark, mErr)) {
                        std::cout << "\n[SUCCESS] Mark recorded successfully! Score: " << score
                                  << " / " << maxScore << " (" << std::fixed << std::setprecision(2)
                                  << newMark.calculatePercentage() << "%)\n";
                    } else {
                        std::cout << "\n[ERROR] " << mErr << "\n";
                    }
                } else if (mOp == 2) {
                    std::cout << "Enter Student ID: ";
                    std::string sId;
                    std::getline(std::cin, sId);
                    std::vector<Mark> marks = repo.getMarksByStudentId(sId);
                    if (marks.empty()) {
                        std::cout << "\n[INFO] No marks recorded for Student ID '" << sId << "'.\n";
                    } else {
                        std::cout << "\nAcademic Marks for Student " << sId << ":\n";
                        std::cout << std::left << std::setw(8) << "ID"
                                  << std::setw(28) << "Subject"
                                  << std::setw(15) << "Assessment"
                                  << std::setw(10) << "Score"
                                  << std::setw(10) << "Max"
                                  << std::setw(12) << "Percentage" << "\n";
                        printDivider();
                        double totalScore = 0.0, totalMax = 0.0;
                        for (const auto& m : marks) {
                            std::cout << std::left << std::setw(8) << m.getMarkId()
                                      << std::setw(28) << m.getSubject()
                                      << std::setw(15) << m.getAssessment()
                                      << std::setw(10) << m.getScore()
                                      << std::setw(10) << m.getMaxScore()
                                      << std::fixed << std::setprecision(2)
                                      << m.calculatePercentage() << "%\n";
                            totalScore += m.getScore();
                            totalMax += m.getMaxScore();
                        }
                        printDivider();
                        double overallPct = (totalMax > 0.0) ? (totalScore / totalMax) * 100.0 : 0.0;
                        std::cout << "Overall Total: " << totalScore << " / " << totalMax
                                  << " (" << std::fixed << std::setprecision(2) << overallPct << "%)\n";
                    }
                } else if (mOp == 3) {
                    std::vector<Mark> allMarks = repo.getAllMarks();
                    std::cout << "\n--- ALL MARKS RECORDS (" << allMarks.size() << ") ---\n";
                    std::cout << std::left << std::setw(8) << "ID"
                              << std::setw(10) << "Student"
                              << std::setw(28) << "Subject"
                              << std::setw(14) << "Assessment"
                              << std::setw(8) << "Score"
                              << std::setw(8) << "Max"
                              << std::setw(10) << "Percentage" << "\n";
                    printDivider();
                    for (const auto& m : allMarks) {
                        std::cout << std::left << std::setw(8) << m.getMarkId()
                                  << std::setw(10) << m.getStudentId()
                                  << std::setw(28) << m.getSubject()
                                  << std::setw(14) << m.getAssessment()
                                  << std::setw(8) << m.getScore()
                                  << std::setw(8) << m.getMaxScore()
                                  << std::fixed << std::setprecision(2)
                                  << m.calculatePercentage() << "%\n";
                    }
                    printDivider();
                }
            } else {
                std::cout << "\n[ERROR] Invalid option selected.\n";
            }
        }
    }

    return 0;
}
