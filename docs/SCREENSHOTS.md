# System Operation Outputs & Screen Walkthroughs

**Project:** Student Record Management System (SRMS)  
**Authors:** Shubham Gupta (Roll No: 45) & Swapnil Yadav (Roll No: 46)  
**Deliverable:** Deliverable #9 (Output Screenshots & Operation Logs)

---

## 1. System Authentication & Banner Screen

```
=================================================================
                 STUDENT RECORD MANAGEMENT SYSTEM                
          Prepared by: Shubham Gupta (45) & Swapnil Yadav (46)   
                 Evaluation 3 - Final Production Release         
=================================================================

>>> SYSTEM ACCESS & AUTHENTICATION <<<
1. Sign In (Administrator / Staff / Teacher)
2. View Default System Credentials
3. About This Project
0. Exit System
Enter Choice: 1
Username: admin
Password: [admin123]

[SUCCESS] Authentication granted! Logged in as: admin (Admin)
```

---

## 2. Administrator Main Dashboard

```
=================================================================
                 STUDENT RECORD MANAGEMENT SYSTEM                
          Prepared by: Shubham Gupta (45) & Swapnil Yadav (46)   
                 Evaluation 3 - Final Production Release         
=================================================================

>>> MAIN DASHBOARD [Logged In: admin | Role: Admin] <<<
1. Student Profile Management (Add, View, Update, Deactivate)
2. Advanced Multi-Criteria Search (ID, Name, Course)
3. Course Management (Add & View Academic Courses)
4. Enrollment Module (Link Students to Courses & Terms)
5. Marks Assessment Module (Score Entry & Auto-Calculation)
6. Attendance Module (Track Daily Attendance & View %)
7. Reports & Academic Transcripts (View & Export to Disk)
8. User & Access Control Management [Admin Only]
9. Logout
0. Exit Application
Select Module: 
```

---

## 3. Student Roster Listing (Module 1, Option 2)

```
--- STUDENT ROSTER (4 Records) ---
ID      Full Name             DOB         Email                   Phone         Course    Status    
-------------------------------------------------------------------------------------------------
S101    Shubham Gupta         2003-05-14  shubham@univ.edu        9988776655    CS101     Active    
S102    Swapnil Yadav         2003-08-22  swapnil@univ.edu        9876543211    CS101     Active    
S103    Aarav Sharma          2003-01-10  aarav@univ.edu          9876543212    IT102     Active    
S199    Rohan Verma           2003-04-12  rohan@univ.edu          9123456780    CS101     Active    
-------------------------------------------------------------------------------------------------
```

---

## 4. Duplicate ID Prevention (TC-04 Validation)

```
>>> REGISTER NEW STUDENT <<<
Student ID (e.g. S105): S101
Full Name: Test Duplicate
Date of Birth (YYYY-MM-DD): 2003-01-01
Email Address: test@univ.edu
Phone Number (7-15 digits): 9999999999
Address: Campus Block
Course ID (e.g. CS101, IT102): CS101

[SYSTEM BLOCKED] Conflict Error: Student ID 'S101' already exists in the system.
```

---

## 5. Marks Boundary Enforcement (TC-06 Validation)

```
--- MARKS ASSESSMENT MODULE ---
1. Record Assessment Mark (Score / Max Score)
2. View Assessment Marks by Student ID
3. View All Assessment Marks
Select Option: 1
Mark ID (e.g. M015): M010
Student ID: S101
Subject Name: Computer Graphics
Assessment Type (Mid-Term, Final Exam, Quiz, Project): Quiz
Maximum Allowed Score (e.g. 100 or 50): 50
Score Obtained: 65

[VALIDATION ERROR] Validation Error: Score (65) exceeds maximum allowed score (50). Record not saved.
```

---

## 6. Official Academic Transcript Output (Module 7)

```
=========================================================================================
                             STUDENT ACADEMIC PERFORMANCE SUMMARY                        
=========================================================================================
Student ID:     S101
Full Name:      Shubham Gupta
Course ID:      CS101
Profile Status: Active
-----------------------------------------------------------------------------------------
Mark ID Subject Name                    Assessment      Score     Max       Percentage  
-----------------------------------------------------------------------------------------
M001    Data Structures & Algorithms    Final Exam      92.5      100       92.50%
M002    Database Management Systems     Final Exam      88.00     100.00    88.00%
M003    Computer Networks               Mid-Term        46.00     50.00     92.00%
-----------------------------------------------------------------------------------------
CUMULATIVE SCORE:      226.50 / 250.00
CUMULATIVE PERCENTAGE: 90.60%
FINAL LETTER GRADE:    A+ (Outstanding)
=========================================================================================
```

---

## 7. Attendance Percentage Calculation (Module 6)

```
Attendance Log for Student S101:
  - 2024-11-01: Present
  - 2024-11-02: Present
  - 2024-11-03: Present
-----------------------------------------------------------------
Total Classes: 3 | Present: 3 | Absent: 0 | Late: 0
Attendance Percentage: 100.0%
-----------------------------------------------------------------
```

---

## 8. Automated Unit Test Verification (All 7 Test Cases)

```
=======================================================================
          STUDENT RECORD MANAGEMENT SYSTEM - AUTOMATED TEST SUITE      
        Executing Verification Plan: TC-01 to TC-07 from Report        
        Authors: Shubham Gupta (45) & Swapnil Yadav (46)               
=======================================================================

Test    Test Scenario                   Expected Result                       Status    
----------------------------------------------------------------------------------------------------
TC-01   Valid Login: Correct creds      Dashboard opens                       [ PASS ]
TC-02   Invalid Login: Wrong pwd        Access denied with a clear message    [ PASS ]
TC-03   Add Student: Complete valid formRecord saved and searchable           [ PASS ]
TC-04   Duplicate ID: Reuse student ID  System blocks duplicate + explanation [ PASS ]
TC-05   Update Profile: Change contact  Updated value is shown after saving   [ PASS ]
TC-06   Invalid Marks: Score > maxScore Validation error; record not saved    [ PASS ]
TC-07   Search Missing ID: Unknown ID   No matching record message            [ PASS ]
----------------------------------------------------------------------------------------------------
SUMMARY: 7 / 7 Test Cases PASSED (100% SUCCESS)
====================================================================================================
```
