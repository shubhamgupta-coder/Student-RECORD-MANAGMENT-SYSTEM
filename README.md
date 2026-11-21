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
| **Evaluation 2** | `v2.0-enhanced` | Version 2: Enrolment system, Marks recording & percentage calculation, Advanced multi-criteria search, strict validation rules. | 🔄 In Progress |
| **Evaluation 3** | `v3.0-final` | Version 3: Full RBAC, Academic reports & transcripts, Soft-deactivation, Attendance tracking, Automated test suite. | ⏳ Planned |

---

## 🚀 Evaluation 1: First Cut / MVP Overview

The MVP demonstrates the core architectural layers:
1. **User Authentication:** Login system supporting Admin and Staff roles with hashed credentials.
2. **Student Entity Management:** Creation and listing of student profiles with validation.
3. **Course Directory:** Catalog of academic courses and departments.
4. **Data Persistence:** Persistent text-based file store (`data/students.txt`, `data/users.txt`, `data/courses.txt`).
5. **Interactive Console UI:** Clean, formatted CLI menu.

### How to Compile and Run (Evaluation 1)
```bash
# Compilation using G++
g++ -std=c++17 -Wall -Iinclude src/main.cpp src/User.cpp src/Course.cpp src/Student.cpp src/StudentRepository.cpp -o srms_v1.exe

# Execute
.\srms_v1.exe
```

### Default Credentials
- **Admin:** `admin` / `admin123`
- **Staff:** `staff` / `staff123`
