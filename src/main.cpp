#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "../include/StudentRepository.h"
#include "../include/User.h"
#include "../include/Student.h"
#include "../include/Course.h"
#include "../include/Enrollment.h"
#include "../include/Mark.h"
#include "../include/Attendance.h"
#include "../include/ReportService.h"
#include "../include/Utils.h"

void printHeader() {
    std::cout << "\n=================================================================\n";
    std::cout << "                 STUDENT RECORD MANAGEMENT SYSTEM                \n";
    std::cout << "          Prepared by: Shubham Gupta (45) & Swapnil Yadav (46)   \n";
    std::cout << "                 Evaluation 3 - Final Production Release         \n";
    std::cout << "=================================================================\n";
}

void printDivider() {
    std::cout << "-----------------------------------------------------------------\n";
}

int main() {
    StudentRepository repo("data");
    ReportService reportService(repo);
    User* currentUser = nullptr;

    while (true) {
        printHeader();

        if (currentUser == nullptr) {
            std::cout << "\n>>> SYSTEM ACCESS & AUTHENTICATION <<<\n";
            std::cout << "1. Sign In (Administrator / Staff / Teacher)\n";
            std::cout << "2. View Default System Credentials\n";
            std::cout << "3. About This Project\n";
            std::cout << "0. Exit System\n";
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
                std::cout << "\nThank you for using the Student Record Management System. Exiting...\n";
                break;
            } else if (authChoice == 2) {
                printDivider();
                std::cout << "[INFO] Default Configured Credentials:\n";
                std::cout << "  Administrator: username: admin | password: admin123  (Full Access)\n";
                std::cout << "  Faculty/Staff: username: staff | password: staff123  (Operations Access)\n";
                printDivider();
                continue;
            } else if (authChoice == 3) {
                printDivider();
                std::cout << "[ABOUT PROJECT]\n";
                std::cout << "  Title:       Student Record Management System (SRMS)\n";
                std::cout << "  Authors:     Shubham Gupta (Roll No: 45) & Swapnil Yadav (Roll No: 46)\n";
                std::cout << "  Language:    C++ (C++17 Standard)\n";
                std::cout << "  Features:    Student Lifecycle, RBAC, Relational Persistence, Marks Calculation,\n";
                std::cout << "               Attendance Tracking, Academic Report Generation & File Export.\n";
                printDivider();
                continue;
            } else if (authChoice == 1) {
                std::string username, password;
                std::cout << "Username: ";
                std::getline(std::cin, username);
                std::cout << "Password: ";
                std::getline(std::cin, password);

                username = Utils::trim(username);
                password = Utils::trim(password);

                User* user = repo.findUserByUsername(username);
                if (user && user->login(password)) {
                    currentUser = user;
                    std::cout << "\n[SUCCESS] Authentication granted! Logged in as: "
                              << currentUser->getUsername() << " (" << currentUser->getRole() << ")\n";
                } else {
                    std::cout << "\n[ERROR] Access Denied: Incorrect username or password.\n";
                }
            }
        } else {
            // Logged in Dashboard
            std::cout << "\n>>> MAIN DASHBOARD [Logged In: " << currentUser->getUsername()
                      << " | Role: " << currentUser->getRole() << "] <<<\n";
            std::cout << "1. Student Profile Management (Add, View, Update, Deactivate)\n";
            std::cout << "2. Advanced Multi-Criteria Search (ID, Name, Course)\n";
            std::cout << "3. Course Management (Add & View Academic Courses)\n";
            std::cout << "4. Enrollment Module (Link Students to Courses & Terms)\n";
            std::cout << "5. Marks Assessment Module (Score Entry & Auto-Calculation)\n";
            std::cout << "6. Attendance Module (Track Daily Attendance & View %)\n";
            std::cout << "7. Reports & Academic Transcripts (View & Export to Disk)\n";
            if (currentUser->getRole() == "Admin") {
                std::cout << "8. User & Access Control Management [Admin Only]\n";
            }
            std::cout << "9. Logout\n";
            std::cout << "0. Exit Application\n";
            std::cout << "Select Module: ";

            int op = -1;
            if (!(std::cin >> op)) {
                std::cin.clear();
                std::string dummy;
                std::getline(std::cin, dummy);
                continue;
            }
            std::cin.ignore(1000, '\n');

            if (op == 0) {
                std::cout << "\nExiting application. Goodbye!\n";
                break;
            } else if (op == 9) {
                currentUser->logout();
                currentUser = nullptr;
                std::cout << "\n[INFO] Session terminated. You have logged out successfully.\n";
            } else if (op == 1) {
                // Student Profile Management Sub-menu
                std::cout << "\n--- STUDENT PROFILE MANAGEMENT ---\n";
                std::cout << "1. Register New Student (Add Record)\n";
                std::cout << "2. View Student Roster (All Active/Inactive)\n";
                std::cout << "3. Update Student Profile (Phone, Address, Course)\n";
                std::cout << "4. Soft Deactivate Student Profile (Preserve Academic History)\n";
                std::cout << "5. Reactivate Student Profile\n";
                std::cout << "6. Delete Student Record Permanently [Admin Only]\n";
                std::cout << "Select Option: ";
                int sOp = -1;
                std::cin >> sOp;
                std::cin.ignore(1000, '\n');

                if (sOp == 1) {
                    std::cout << "\n>>> REGISTER NEW STUDENT <<<\n";
                    std::string sId, name, dob, email, phone, address, courseId;

                    std::cout << "Student ID (e.g. S105): ";
                    std::getline(std::cin, sId);
                    std::cout << "Full Name: ";
                    std::getline(std::cin, name);
                    std::cout << "Date of Birth (YYYY-MM-DD): ";
                    std::getline(std::cin, dob);
                    std::cout << "Email Address: ";
                    std::getline(std::cin, email);
                    std::cout << "Phone Number (7-15 digits): ";
                    std::getline(std::cin, phone);
                    std::cout << "Address: ";
                    std::getline(std::cin, address);
                    std::cout << "Course ID (e.g. CS101, IT102): ";
                    std::getline(std::cin, courseId);

                    Student newStudent(Utils::trim(sId), Utils::trim(name), Utils::trim(dob),
                                       Utils::trim(email), Utils::trim(phone), Utils::trim(address),
                                       Utils::trim(courseId), "Active");

                    std::string valErr;
                    if (!newStudent.validateDetails(valErr)) {
                        std::cout << "\n[VALIDATION REJECTED] " << valErr << "\n";
                    } else {
                        std::string repoErr;
                        if (repo.add(newStudent, repoErr)) {
                            std::cout << "\n[SUCCESS] Student record saved successfully! Assigned ID: "
                                      << newStudent.getStudentId() << "\n";
                        } else {
                            std::cout << "\n[SYSTEM BLOCKED] " << repoErr << "\n";
                        }
                    }
                } else if (sOp == 2) {
                    std::cout << "\nShow: 1. All Records  2. Only Active Records: ";
                    int filterOpt = 1;
                    std::cin >> filterOpt;
                    std::cin.ignore(1000, '\n');

                    std::vector<Student> students = repo.getAllStudents(filterOpt == 1);
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
                    // Update profile
                    std::cout << "\n>>> UPDATE STUDENT PROFILE <<<\n";
                    std::cout << "Enter Student ID to update: ";
                    std::string targetId;
                    std::getline(std::cin, targetId);
                    Student* existing = repo.findById(targetId);
                    if (!existing) {
                        std::cout << "\n[ERROR] Student ID '" << targetId << "' not found.\n";
                    } else {
                        std::cout << "Editing profile: " << existing->getFullName() << " (" << existing->getStudentId() << ")\n";
                        std::cout << "Current Phone: " << existing->getPhone() << " -> New phone (Enter to keep): ";
                        std::string newPhone;
                        std::getline(std::cin, newPhone);
                        if (!Utils::trim(newPhone).empty()) existing->updateProfile("", "", "", Utils::trim(newPhone), "", "");

                        std::cout << "Current Address: " << existing->getAddress() << " -> New address (Enter to keep): ";
                        std::string newAddr;
                        std::getline(std::cin, newAddr);
                        if (!Utils::trim(newAddr).empty()) existing->updateProfile("", "", "", "", Utils::trim(newAddr), "");

                        std::cout << "Current Course: " << existing->getCourseId() << " -> New course ID (Enter to keep): ";
                        std::string newCId;
                        std::getline(std::cin, newCId);
                        if (!Utils::trim(newCId).empty()) existing->setCourseId(Utils::trim(newCId));

                        std::string updateErr;
                        if (repo.update(*existing, updateErr)) {
                            std::cout << "\n[SUCCESS] Profile updated successfully!\n";
                            std::cout << "Updated values verified in database:\n";
                            std::cout << "  Phone:   " << existing->getPhone() << "\n";
                            std::cout << "  Address: " << existing->getAddress() << "\n";
                            std::cout << "  Course:  " << existing->getCourseId() << "\n";
                        } else {
                            std::cout << "\n[ERROR] " << updateErr << "\n";
                        }
                    }
                } else if (sOp == 4) {
                    // Soft deactivation (Section 6.3)
                    std::cout << "\n>>> DEACTIVATE STUDENT PROFILE <<<\n";
                    std::cout << "Enter Student ID to deactivate: ";
                    std::string dId;
                    std::getline(std::cin, dId);
                    std::string dErr;
                    if (repo.deactivate(dId, dErr)) {
                        std::cout << "\n[SUCCESS] Student profile set to 'Inactive'. Historical marks and enrollment preserved.\n";
                    } else {
                        std::cout << "\n[ERROR] " << dErr << "\n";
                    }
                } else if (sOp == 5) {
                    // Reactivation
                    std::cout << "\n>>> REACTIVATE STUDENT PROFILE <<<\n";
                    std::cout << "Enter Student ID to reactivate: ";
                    std::string rId;
                    std::getline(std::cin, rId);
                    std::string rErr;
                    if (repo.activate(rId, rErr)) {
                        std::cout << "\n[SUCCESS] Student profile set back to 'Active'.\n";
                    } else {
                        std::cout << "\n[ERROR] " << rErr << "\n";
                    }
                } else if (sOp == 6) {
                    // Hard Delete (Admin Only)
                    if (currentUser->getRole() != "Admin") {
                        std::cout << "\n[PERMISSION DENIED] Only Administrators can permanently delete student records.\n";
                    } else {
                        std::cout << "\n>>> PERMANENT RECORD DELETION [Admin Only] <<<\n";
                        std::cout << "Enter Student ID to delete: ";
                        std::string delId;
                        std::getline(std::cin, delId);
                        delId = Utils::trim(delId);

                        std::cout << "Are you sure you want to permanently delete student " << delId << "? (y/N): ";
                        std::string confirm;
                        std::getline(std::cin, confirm);
                        if (confirm == "y" || confirm == "Y") {
                            std::string delErr;
                            if (repo.remove(delId, delErr)) {
                                std::cout << "\n[SUCCESS] Student record " << delId << " permanently removed from database.\n";
                            } else {
                                std::cout << "\n[ERROR] " << delErr << "\n";
                            }
                        } else {
                            std::cout << "\n[CANCELLED] Deletion cancelled by user.\n";
                        }
                    }
                }
            } else if (op == 2) {
                // Advanced search
                std::cout << "\n--- ADVANCED MULTI-CRITERIA SEARCH ---\n";
                std::cout << "Enter keyword (Student ID, Full Name, or Course Code): ";
                std::string keyword;
                std::getline(std::cin, keyword);
                keyword = Utils::trim(keyword);

                std::vector<Student> matches = repo.search(keyword);
                if (matches.empty()) {
                    std::cout << "\n[INFO] No matching record message: No student records match '" << keyword << "'.\n";
                } else {
                    std::cout << "\nFound " << matches.size() << " Matching Record(s):\n";
                    printDivider();
                    for (const auto& s : matches) {
                        std::cout << "ID: " << std::left << std::setw(8) << s.getStudentId()
                                  << " | Name: " << std::setw(20) << s.getFullName()
                                  << " | Course: " << std::setw(8) << s.getCourseId()
                                  << " | Phone: " << std::setw(14) << s.getPhone()
                                  << " | Status: " << s.getStatus() << "\n";
                    }
                    printDivider();
                }
            } else if (op == 3) {
                // Course Management
                std::cout << "\n--- COURSE DIRECTORY MANAGEMENT ---\n";
                std::cout << "1. Add New Academic Course\n";
                std::cout << "2. View All Available Courses\n";
                std::cout << "Select Option: ";
                int cOp = -1;
                std::cin >> cOp;
                std::cin.ignore(1000, '\n');

                if (cOp == 1) {
                    std::string cId, cName, dept;
                    int dur = 0;
                    std::cout << "Course Code / ID (e.g. AI105): ";
                    std::getline(std::cin, cId);
                    std::cout << "Course Full Name: ";
                    std::getline(std::cin, cName);
                    std::cout << "Department: ";
                    std::getline(std::cin, dept);
                    std::cout << "Duration in Years (1-6): ";
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
                // Enrollment module
                std::cout << "\n--- ENROLLMENT MANAGEMENT MODULE ---\n";
                std::cout << "1. Enroll Student to Course / Term\n";
                std::cout << "2. View All Enrollments\n";
                std::cout << "3. View Enrollment History by Student ID\n";
                std::cout << "4. Change Student Enrolled Course\n";
                std::cout << "Select Option: ";
                int eOp = -1;
                std::cin >> eOp;
                std::cin.ignore(1000, '\n');

                if (eOp == 1) {
                    std::string eId, sId, cId, year, sem, date;
                    std::cout << "Enrollment ID (e.g. E005): ";
                    std::getline(std::cin, eId);
                    std::cout << "Student ID: ";
                    std::getline(std::cin, sId);
                    std::cout << "Course ID: ";
                    std::getline(std::cin, cId);
                    std::cout << "Academic Year (e.g. 2024-2025): ";
                    std::getline(std::cin, year);
                    std::cout << "Semester (e.g. Semester 5): ";
                    std::getline(std::cin, sem);
                    std::cout << "Enrollment Date (YYYY-MM-DD): ";
                    std::getline(std::cin, date);

                    Enrollment newEnr(Utils::trim(eId), Utils::trim(sId), Utils::trim(cId),
                                      Utils::trim(year), Utils::trim(sem), Utils::trim(date));
                    std::string eErr;
                    if (repo.addEnrollment(newEnr, eErr)) {
                        std::cout << "\n[SUCCESS] Student " << sId << " successfully enrolled in course " << cId << "!\n";
                    } else {
                        std::cout << "\n[REJECTED] " << eErr << "\n";
                    }
                } else if (eOp == 2) {
                    std::vector<Enrollment> enrollments = repo.getAllEnrollments();
                    std::cout << "\n--- ENROLLMENT REGISTRY (" << enrollments.size() << " Entries) ---\n";
                    std::cout << std::left << std::setw(12) << "Enroll ID"
                              << std::setw(12) << "Student ID"
                              << std::setw(12) << "Course ID"
                              << std::setw(16) << "Academic Year"
                              << std::setw(14) << "Semester"
                              << std::setw(12) << "Date" << "\n";
                    printDivider();
                    for (const auto& e : enrollments) {
                        std::cout << std::left << std::setw(12) << e.getEnrollmentId()
                                  << std::setw(12) << e.getStudentId()
                                  << std::setw(12) << e.getCourseId()
                                  << std::setw(16) << e.getAcademicYear()
                                  << std::setw(14) << e.getSemester()
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
                        std::cout << "\nEnrollment Records for Student " << sId << ":\n";
                        for (const auto& e : list) {
                            std::cout << "  - " << e.getEnrollmentDetails() << "\n";
                        }
                    }
                } else if (eOp == 4) {
                    // Change enrolled course
                    std::cout << "\n>>> CHANGE STUDENT ENROLLED COURSE <<<\n";
                    std::cout << "Enter Student ID: ";
                    std::string sId;
                    std::getline(std::cin, sId);
                    Student* s = repo.findById(sId);
                    if (!s) {
                        std::cout << "\n[ERROR] Student ID '" << sId << "' not found.\n";
                    } else {
                        std::cout << "Current Student Course: " << s->getCourseId() << "\n";
                        std::cout << "Enter New Course ID: ";
                        std::string newCId;
                        std::getline(std::cin, newCId);
                        newCId = Utils::trim(newCId);

                        Course* c = repo.findCourseById(newCId);
                        if (!c) {
                            std::cout << "\n[ERROR] Course ID '" << newCId << "' does not exist in course catalog.\n";
                        } else {
                            s->setCourseId(newCId);
                            std::string uErr;
                            if (repo.update(*s, uErr)) {
                                std::cout << "\n[SUCCESS] Student " << sId << " course successfully updated to: " << newCId << " (" << c->getCourseName() << ")!\n";
                            } else {
                                std::cout << "\n[ERROR] " << uErr << "\n";
                            }
                        }
                    }
                }
            } else if (op == 5) {
                // Marks Assessment
                std::cout << "\n--- MARKS ASSESSMENT MODULE ---\n";
                std::cout << "1. Record Assessment Mark (Score / Max Score)\n";
                std::cout << "2. View Assessment Marks by Student ID\n";
                std::cout << "3. View All Assessment Marks\n";
                std::cout << "Select Option: ";
                int mOp = -1;
                std::cin >> mOp;
                std::cin.ignore(1000, '\n');

                if (mOp == 1) {
                    std::string mId, sId, subject, assess;
                    double score = 0.0, maxScore = 100.0;

                    std::cout << "Mark ID (e.g. M015): ";
                    std::getline(std::cin, mId);
                    std::cout << "Student ID: ";
                    std::getline(std::cin, sId);
                    std::cout << "Subject Name: ";
                    std::getline(std::cin, subject);
                    std::cout << "Assessment Type (Mid-Term, Final Exam, Quiz, Project): ";
                    std::getline(std::cin, assess);
                    std::cout << "Maximum Allowed Score (e.g. 100 or 50): ";
                    std::cin >> maxScore;
                    std::cout << "Score Obtained: ";
                    std::cin >> score;
                    std::cin.ignore(1000, '\n');

                    Mark newMark(Utils::trim(mId), Utils::trim(sId), Utils::trim(subject),
                                 Utils::trim(assess), score, maxScore);

                    std::string mErr;
                    if (!newMark.validateScore(mErr)) {
                        std::cout << "\n[VALIDATION ERROR] " << mErr << " Record not saved.\n";
                    } else if (repo.addMark(newMark, mErr)) {
                        std::cout << "\n[SUCCESS] Mark recorded successfully! Score: " << score
                                  << " / " << maxScore << " (" << std::fixed << std::setprecision(2)
                                  << newMark.calculatePercentage() << "%)\n";
                    } else {
                        std::cout << "\n[REJECTED] " << mErr << "\n";
                    }
                } else if (mOp == 2) {
                    std::cout << "Enter Student ID: ";
                    std::string sId;
                    std::getline(std::cin, sId);
                    std::cout << reportService.marksSummary(sId);
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
            } else if (op == 6) {
                // Attendance Module (Bonus from Deliverables Doc 1)
                std::cout << "\n--- ATTENDANCE TRACKING MODULE ---\n";
                std::cout << "1. Mark Daily Attendance (Present / Absent / Late)\n";
                std::cout << "2. View Student Attendance Summary & Percentage\n";
                std::cout << "3. View All Attendance Entries\n";
                std::cout << "Select Option: ";
                int aOp = -1;
                std::cin >> aOp;
                std::cin.ignore(1000, '\n');

                if (aOp == 1) {
                    std::string aId, sId, date, status;
                    std::cout << "Attendance ID (e.g. A010): ";
                    std::getline(std::cin, aId);
                    std::cout << "Student ID: ";
                    std::getline(std::cin, sId);
                    std::cout << "Date (YYYY-MM-DD): ";
                    std::getline(std::cin, date);
                    std::cout << "Status (Present / Absent / Late): ";
                    std::getline(std::cin, status);

                    Attendance att(Utils::trim(aId), Utils::trim(sId), Utils::trim(date), Utils::trim(status));
                    std::string aErr;
                    if (repo.addAttendance(att, aErr)) {
                        std::cout << "\n[SUCCESS] Attendance recorded for Student " << sId << " on " << date << " (" << status << ")!\n";
                    } else {
                        std::cout << "\n[ERROR] " << aErr << "\n";
                    }
                } else if (aOp == 2) {
                    std::cout << "Enter Student ID: ";
                    std::string sId;
                    std::getline(std::cin, sId);
                    std::vector<Attendance> list = repo.getAttendanceByStudentId(sId);
                    if (list.empty()) {
                        std::cout << "\n[INFO] No attendance records found for Student ID '" << sId << "'.\n";
                    } else {
                        int presentCount = 0;
                        int absentCount = 0;
                        int lateCount = 0;

                        std::cout << "\nAttendance Log for Student " << sId << ":\n";
                        for (const auto& a : list) {
                            std::cout << "  - " << a.getDate() << ": " << a.getStatus() << "\n";
                            if (a.getStatus() == "Present") presentCount++;
                            else if (a.getStatus() == "Absent") absentCount++;
                            else if (a.getStatus() == "Late") lateCount++;
                        }
                        int total = list.size();
                        double attPct = (total > 0) ? (static_cast<double>(presentCount) / total) * 100.0 : 0.0;
                        printDivider();
                        std::cout << "Total Classes: " << total
                                  << " | Present: " << presentCount
                                  << " | Absent: " << absentCount
                                  << " | Late: " << lateCount << "\n";
                        std::cout << "Attendance Percentage: " << std::fixed << std::setprecision(1) << attPct << "%\n";
                        printDivider();
                    }
                } else if (aOp == 3) {
                    std::vector<Attendance> allAtt = repo.getAllAttendance();
                    std::cout << "\n--- ALL ATTENDANCE LOGS (" << allAtt.size() << ") ---\n";
                    std::cout << std::left << std::setw(12) << "Record ID"
                              << std::setw(12) << "Student ID"
                              << std::setw(15) << "Date"
                              << std::setw(12) << "Status" << "\n";
                    printDivider();
                    for (const auto& a : allAtt) {
                        std::cout << std::left << std::setw(12) << a.getAttendanceId()
                                  << std::setw(12) << a.getStudentId()
                                  << std::setw(15) << a.getDate()
                                  << std::setw(12) << a.getStatus() << "\n";
                    }
                    printDivider();
                }
            } else if (op == 7) {
                // Reports & Transcripts
                std::cout << "\n--- REPORTS & ACADEMIC TRANSCRIPTS ---\n";
                std::cout << "1. Display Class Roster Report\n";
                std::cout << "2. Export Class Roster Report to File (reports/student_roster.txt)\n";
                std::cout << "3. Display Student Academic Performance Summary\n";
                std::cout << "4. Export Student Official Transcript to File (reports/transcript_<id>.txt)\n";
                std::cout << "Select Option: ";
                int rOp = -1;
                std::cin >> rOp;
                std::cin.ignore(1000, '\n');

                if (rOp == 1) {
                    std::cout << reportService.studentList();
                } else if (rOp == 2) {
                    std::string content = reportService.studentList();
                    std::string filename = "reports/student_roster.txt";
                    std::string err;
                    if (reportService.exportReport(content, filename, err)) {
                        std::cout << "\n[SUCCESS] Class Roster successfully exported to: " << filename << "\n";
                    } else {
                        std::cout << "\n[ERROR] " << err << "\n";
                    }
                } else if (rOp == 3) {
                    std::cout << "Enter Student ID: ";
                    std::string sId;
                    std::getline(std::cin, sId);
                    std::cout << reportService.marksSummary(sId);
                } else if (rOp == 4) {
                    std::cout << "Enter Student ID: ";
                    std::string sId;
                    std::getline(std::cin, sId);
                    std::string content = reportService.marksSummary(sId);
                    std::string filename = "reports/transcript_" + sId + ".txt";
                    std::string err;
                    if (reportService.exportReport(content, filename, err)) {
                        std::cout << "\n[SUCCESS] Official Transcript exported to: " << filename << "\n";
                    } else {
                        std::cout << "\n[ERROR] " << err << "\n";
                    }
                }
            } else if (op == 8) {
                // User & Access Control (Admin Only)
                if (currentUser->getRole() != "Admin") {
                    std::cout << "\n[SECURITY WARNING] Access Denied! Administrative privilege required.\n";
                } else {
                    std::cout << "\n--- USER & ACCESS CONTROL MANAGEMENT [Admin Only] ---\n";
                    std::cout << "1. Create New System User Account\n";
                    std::cout << "2. View All System Accounts\n";
                    std::cout << "3. Toggle User Active/Inactive Status\n";
                    std::cout << "Select Option: ";
                    int uOp = -1;
                    std::cin >> uOp;
                    std::cin.ignore(1000, '\n');

                    if (uOp == 1) {
                        std::string uId, uName, uPwd, uRole;
                        std::cout << "User ID (e.g. U003): ";
                        std::getline(std::cin, uId);
                        std::cout << "Username: ";
                        std::getline(std::cin, uName);
                        std::cout << "Password: ";
                        std::getline(std::cin, uPwd);
                        std::cout << "Role (Admin / Staff): ";
                        std::getline(std::cin, uRole);

                        User newUser(Utils::trim(uId), Utils::trim(uName),
                                     Utils::hashPassword(Utils::trim(uPwd)),
                                     Utils::trim(uRole), true);
                        std::string uErr;
                        if (repo.addUser(newUser, uErr)) {
                            std::cout << "\n[SUCCESS] User account '" << uName << "' created with role '" << uRole << "'.\n";
                        } else {
                            std::cout << "\n[ERROR] " << uErr << "\n";
                        }
                    } else if (uOp == 2) {
                        std::vector<User> users = repo.getAllUsers();
                        std::cout << "\n--- SYSTEM USERS (" << users.size() << ") ---\n";
                        std::cout << std::left << std::setw(10) << "User ID"
                                  << std::setw(18) << "Username"
                                  << std::setw(12) << "Role"
                                  << std::setw(10) << "Active" << "\n";
                        printDivider();
                        for (const auto& u : users) {
                            std::cout << std::left << std::setw(10) << u.getUserId()
                                      << std::setw(18) << u.getUsername()
                                      << std::setw(12) << u.getRole()
                                      << (u.isActive() ? "Yes" : "No") << "\n";
                        }
                        printDivider();
                    } else if (uOp == 3) {
                        std::cout << "Enter Username to toggle status: ";
                        std::string uName;
                        std::getline(std::cin, uName);
                        User* u = repo.findUserByUsername(uName);
                        if (!u) {
                            std::cout << "\n[ERROR] User '" << uName << "' not found.\n";
                        } else if (u->getUsername() == currentUser->getUsername()) {
                            std::cout << "\n[ERROR] Cannot deactivate your own active session!\n";
                        } else {
                            u->setActive(!u->isActive());
                            std::cout << "\n[SUCCESS] User '" << u->getUsername() << "' status toggled to: "
                                      << (u->isActive() ? "Active" : "Inactive") << "\n";
                        }
                    }
                }
            } else {
                std::cout << "\n[ERROR] Invalid option selected. Please choose from menu.\n";
            }
        }
    }

    return 0;
}
