# Entity-Relationship (ER) Diagram & Data Dictionary

**Project:** Student Record Management System (SRMS)  
**Authors:** Shubham Gupta (Roll No: 45) & Swapnil Yadav (Roll No: 46)  
**Deliverable:** Deliverable #3 (ER Diagram & Entity Relationships)

---

## 1. Visual Entity-Relationship Diagram (Mermaid)

```mermaid
erDiagram
    USERS {
        string user_id PK "Unique User Identifier"
        string username UK "Unique Username for Login"
        string password_hash "Salted Cryptographic Hash"
        string role "Admin or Staff"
        boolean active "Account Status"
    }

    STUDENT {
        string student_id PK "Unique Student Identifier"
        string full_name "Student Full Name"
        date dob "Date of Birth (YYYY-MM-DD)"
        string email UK "Student Institutional Email"
        string phone "Contact Phone Number"
        string address "Residential Address"
        string course_id FK "Enrolled Course Reference"
        string status "Active or Inactive"
    }

    COURSE {
        string course_id PK "Unique Course Code"
        string course_name "Descriptive Program Name"
        string department "Academic Department"
        int duration "Duration in Years (1 to 6)"
    }

    ENROLLMENT {
        string enrollment_id PK "Unique Enrollment Identifier"
        string student_id FK "References STUDENT(student_id)"
        string course_id FK "References COURSE(course_id)"
        string academic_year "e.g. 2024-2025"
        string semester "e.g. Semester 5"
        date enrollment_date "Date of Registration"
    }

    MARKS {
        string mark_id PK "Unique Assessment Mark ID"
        string student_id FK "References STUDENT(student_id)"
        string subject "Academic Subject Name"
        string assessment "Assessment Type (Mid-Term, Final, Quiz)"
        decimal score "Score Obtained (>= 0)"
        decimal max_score "Maximum Possible Score (> 0)"
    }

    ATTENDANCE {
        string attendance_id PK "Unique Attendance ID"
        string student_id FK "References STUDENT(student_id)"
        date attendance_date "Date of Class (YYYY-MM-DD)"
        string status "Present, Absent, or Late"
    }

    STUDENT ||--o{ ENROLLMENT : "enrolls in (1:N)"
    COURSE ||--o{ ENROLLMENT : "includes (1:N)"
    STUDENT ||--o{ MARKS : "awarded (1:N)"
    STUDENT ||--o{ ATTENDANCE : "logged for (1:N)"
```

---

## 2. Cardinality & Relational Mappings

```
  +--------------+               1 : N              +------------------+
  |   STUDENT    | -------------------------------> |    ENROLLMENT    |
  +--------------+                                  +------------------+
         |                                                   ^
         | 1 : N                                             | N : 1
         v                                                   |
  +--------------+                                  +------------------+
  |    MARKS     |                                  |      COURSE      |
  +--------------+                                  +------------------+
         ^
         |
  +--------------+                                  +------------------+
  |  ATTENDANCE  | <--- (1 : N from STUDENT)        |      USERS       |
  +--------------+                                  +------------------+
```

### Relational Rules:
1. **STUDENT to ENROLLMENT (1 to Many):**
   - One student can be enrolled in multiple semesters/terms over their academic program.
   - An enrollment record belongs strictly to exactly one student.
2. **COURSE to ENROLLMENT (1 to Many):**
   - One course program can have many student enrollments across terms.
   - Each enrollment record references a valid course code in the catalog.
3. **STUDENT to MARKS (1 to Many):**
   - One student has multiple marks entries across various subjects and assessment periods.
   - Every mark entry is linked to an existing student ID.
4. **STUDENT to ATTENDANCE (1 to Many):**
   - Daily attendance records link to a specific student ID with timestamps and attendance states.
5. **USERS (Access Control):**
   - Independent entity maintaining administrator and faculty/staff authentication credentials and permission flags.
