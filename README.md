# Student Record Management System (SRMS)

**Academic Project Documentation & Software Implementation**  
**Core Language:** C++ (Standard C++17)  
**Compilation Toolchain:** GCC / MinGW-w64 16.2.0

### Prepared by:
- **Shubham Gupta** (Roll No: **45**)
- **Swapnil Yadav** (Roll No: **46**)

---

## 📌 Project Milestones & Evaluation Git Tags

| Evaluation Milestone | Git Release Tag | Key Deliverables & Implementation Highlights | Status |
|---|---|---|---|
| **Evaluation 1** | `v1.0-mvp` | **First Cut / MVP Version:** Core OOP classes (`User`, `Student`, `Course`), Authentication system, Add student, View roster, Search by ID, and initial file persistence. | ✅ Completed |
| **Evaluation 2** | `v2.0-enhanced` | **Version 2 (Intermediate):** Enrolment service, Marks assessment service, Auto-calculated percentages, Multi-criteria search (ID/Name/Course), Profile updates, and Input validation rules (TC-05, TC-06). | ✅ Completed |
| **Evaluation 3** | `v3.0-final` | **Version 3 (Final Release):** Full Role-Based Access Control (Admin vs. Staff), Soft-deactivation (preserving history), Attendance tracking, Academic Transcripts & Report Export to disk, Automated Test Suite (TC-01 to TC-07). | ✅ Completed |

---

## 🏛 System Architecture (3-Tier Model)

The application follows a clean 3-tier object-oriented architecture:
1. **Presentation Layer:** Interactive command-line interface with formatted tables, input prompts, status feedback, and administrative dashboards.
2. **Application / Business Logic Layer:**
   - `User` / `AuthService`: Credential verification and role-based permissions (Admin vs. Staff).
   - `StudentService`: Validation of student details, lifecycle status, contact updates.
   - `CourseService`: Course code registry and department allocation.
   - `EnrollmentService`: Enrolling students into courses and terms with foreign key integrity.
   - `MarksService`: Recording scores, enforcing boundary limits ($0 \le score \le max\_score$).
   - `ReportService`: Formatting academic summaries, grade calculation, and disk export.
3. **Data Layer (Relational File Store):**
   - `data/users.txt` (Hashed passwords, roles, active flags)
   - `data/students.txt` (Student profiles, contact info, status)
   - `data/courses.txt` (Course catalog, departments, durations)
   - `data/enrollments.txt` (Student-course links, academic years, semesters)
   - `data/marks.txt` (Assessment results, scores, max scores)
   - `data/attendance.txt` (Daily attendance records)

---

## 📊 Database Schema & Entity Relationships

```
+------------+       1:N       +----------------+       N:1       +------------+
|  STUDENT   | <-------------> |   ENROLLMENT   | <-------------> |   COURSE   |
+------------+                 +----------------+                 +------------+
      |
      | 1:N
      v
+------------+                 +----------------+                 +------------+
|   MARKS    |                 |   ATTENDANCE   |                 |   USERS    |
+------------+                 +----------------+                 +------------+
```

### Table Definitions:
1. **USERS:** `user_id (PK) | username (UK) | password_hash | role | active`
2. **STUDENT:** `student_id (PK) | full_name | dob | email | phone | address | course_id | status`
3. **COURSE:** `course_id (PK) | course_name | department | duration_years`
4. **ENROLLMENT:** `enrollment_id (PK) | student_id (FK) | course_id (FK) | academic_year | semester | date`
5. **MARKS:** `mark_id (PK) | student_id (FK) | subject | assessment | score | max_score`
6. **ATTENDANCE:** `attendance_id (PK) | student_id (FK) | date | status`

---

## 🧪 Quality Assurance & Test Verification Matrix

All 7 test cases specified in Table 11 of the project report are validated by an automated unit test suite (`tests/test_srms.cpp`):

| Test ID | Test Scenario | Input / Action | Expected Result | Actual Result | Verification |
|---|---|---|---|---|---|
| **TC-01** | Valid Login | Correct username & password (`admin` / `admin123`) | Dashboard opens | Access granted, user session initialized | **PASS** |
| **TC-02** | Invalid Login | Incorrect password (`wrongpwd`) | Access denied with a clear message | Access denied, system blocks unauthorized login | **PASS** |
| **TC-03** | Add Student | Complete valid form with new ID (`S199`) | Record saved and searchable | Record written to disk and queryable | **PASS** |
| **TC-04** | Duplicate ID | Reuse existing student ID (`S101`) | System blocks duplicate and explains why | Blocked with "Student ID already exists" | **PASS** |
| **TC-05** | Update Profile | Change phone number or address | Updated value is shown after saving | Modified values persisted in database | **PASS** |
| **TC-06** | Invalid Marks | Score greater than maximum ($110 > 100$) | Validation error; record not saved | Blocked with "Score exceeds maximum score" | **PASS** |
| **TC-07** | Search Missing ID | Query non-existent ID (`S_NON_EXISTENT`) | No matching record message | Returns "No matching record found" | **PASS** |

---

## 🛠 Compilation & Execution Instructions

### 1. Compile the Main Application
```bash
# Using build script (Windows)
.\build.bat

# Or direct compilation using G++:
g++ -std=c++17 -Wall -O2 -Iinclude src/main.cpp src/User.cpp src/Course.cpp src/Student.cpp src/Enrollment.cpp src/Mark.cpp src/Attendance.cpp src/ReportService.cpp src/StudentRepository.cpp -o srms.exe
```

### 2. Run the Main Application
```bash
.\srms.exe
```

### 3. Run the Automated Test Suite
```bash
# Compile and run test suite:
g++ -std=c++17 -Wall -Iinclude tests/test_srms.cpp src/User.cpp src/Course.cpp src/Student.cpp src/Enrollment.cpp src/Mark.cpp src/Attendance.cpp src/ReportService.cpp src/StudentRepository.cpp -o test_suite.exe

.\test_suite.exe
```

---

## 🔑 Default Credentials

- **Administrator:** `username: admin` | `password: admin123`
- **Faculty / Staff:** `username: staff` | `password: staff123`

---

## 📂 Exported Reports Directory
Exported academic transcripts and class rosters are stored in `reports/`:
- `reports/student_roster.txt`: Class roster list with complete demographic data.
- `reports/transcript_<studentId>.txt`: Official academic transcript with marks breakdown, cumulative percentage, and letter grade (A+, A, B, C, D, F).

---

## 🚀 Pushing Code to GitHub

To push this repository to your GitHub account:
```bash
# 1. Add your remote repository URL (replace with your repo URL):
git remote add origin https://github.com/<your-username>/<your-repo-name>.git

# 2. Rename branch to main (if needed):
git branch -M main

# 3. Push all commits along with release tags (v1.0-mvp, v2.0-enhanced, v3.0-final):
git push -u origin main --tags
```
