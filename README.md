# Student Record Management System (SRMS)

**Course Project - Academic Documentation & Software Implementation**  
**Language:** C++ (Standard C++17)  

### Project Authors & Roll Numbers
- **Shubham Gupta** (Roll No: 45)
- **Swapnil Yadav** (Roll No: 46)

---

## 📌 Project Milestones & Git Release Tags

| Milestone | Tag | Description | Status |
|---|---|---|---|
| **Evaluation 1** | `v1.0-mvp` | First Cut / Minimum Viable Product (MVP): Core OOP classes (`User`, `Student`, `Course`), authentication, add student, view roster, search by ID, persistence. | ✅ Completed |
| **Evaluation 2** | `v2.0-enhanced` | Version 2: Enrolment system, Marks recording & percentage calculation, Advanced multi-criteria search, profile updates, strict validation rules (TC-05, TC-06). | ✅ Completed |
| **Evaluation 3** | `v3.0-final` | Version 3: Full RBAC, Academic reports & transcripts, Soft-deactivation, Attendance tracking, Automated test suite. | 🔄 In Progress |

---

## 🚀 Evaluation 2: Version 2 Release Notes

Evaluation 2 introduces major business logic modules based on the project design specification:
1. **Enrollment Service:** Link student IDs to courses, academic years, semesters, and enrollment dates with Foreign Key integrity.
2. **Marks & Assessment Service:** Record assessment scores, calculate percentages, and enforce constraint $0 \le score \le max\_score$ (TC-06).
3. **Advanced Multi-Criteria Search:** Instant query across Student ID, Full Name, and Course codes.
4. **Student Profile Update:** Modify contact details (phone, address) with immediate reflection (TC-05).
5. **Multi-Table Relational Data Store:** Relational text databases (`data/students.txt`, `data/courses.txt`, `data/enrollments.txt`, `data/marks.txt`, `data/users.txt`).

### How to Compile and Run (Evaluation 2)
```bash
# Compilation using G++
g++ -std=c++17 -Wall -Iinclude src/main.cpp src/User.cpp src/Course.cpp src/Student.cpp src/Enrollment.cpp src/Mark.cpp src/StudentRepository.cpp -o srms_v2.exe

# Execute
.\srms_v2.exe
```

### Default Credentials
- **Admin:** `admin` / `admin123`
- **Staff:** `staff` / `staff123`
