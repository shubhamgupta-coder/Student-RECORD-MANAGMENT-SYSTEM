import sys
from pptx import Presentation
from pptx.util import Inches, Pt
from pptx.enum.text import PP_ALIGN
from pptx.dml.color import RGBColor
from pptx.enum.shapes import MSO_SHAPE

def create_presentation():
    prs = Presentation()
    # Set 16:9 widescreen dimensions
    prs.slide_width = Inches(13.333)
    prs.slide_height = Inches(7.5)
    blank_layout = prs.slide_layouts[6]

    # Professional Color Palette
    PRIMARY = RGBColor(11, 37, 69)      # #0B2545 Deep Navy
    SECONDARY = RGBColor(19, 78, 94)    # #134E5E Teal Blue
    ACCENT = RGBColor(0, 180, 216)      # #00B4D8 Electric Cyan / Accent
    DARK_TEXT = RGBColor(30, 41, 59)    # #1E293B Charcoal text
    MUTED_TEXT = RGBColor(100, 116, 139)# #64748B Slate gray
    WHITE = RGBColor(255, 255, 255)
    LIGHT_BG = RGBColor(248, 250, 252)  # #F8FAFC
    CARD_BG = RGBColor(241, 245, 249)   # #F1F5F9
    BORDER_COLOR = RGBColor(203, 213, 225)
    SUCCESS = RGBColor(16, 149, 100)    # Green

    def add_header(slide, title_text, category_text="ACADEMIC PROJECT PRESENTATION"):
        # Top banner background line
        top_bar = slide.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0), Inches(0), Inches(13.333), Inches(0.12))
        top_bar.fill.solid()
        top_bar.fill.fore_color.rgb = ACCENT
        top_bar.line.color.rgb = ACCENT

        # Category / Subtitle
        cat_box = slide.shapes.add_textbox(Inches(0.8), Inches(0.35), Inches(11.5), Inches(0.35))
        tf_cat = cat_box.text_frame
        tf_cat.word_wrap = True
        p_cat = tf_cat.paragraphs[0]
        p_cat.text = category_text.upper()
        p_cat.font.size = Pt(10)
        p_cat.font.bold = True
        p_cat.font.color.rgb = ACCENT

        # Title
        title_box = slide.shapes.add_textbox(Inches(0.8), Inches(0.65), Inches(11.5), Inches(0.8))
        tf_title = title_box.text_frame
        tf_title.word_wrap = True
        p_title = tf_title.paragraphs[0]
        p_title.text = title_text
        p_title.font.size = Pt(24)
        p_title.font.bold = True
        p_title.font.color.rgb = PRIMARY

        # Bottom footer line
        footer_line = slide.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0.8), Inches(6.9), Inches(11.733), Inches(0.02))
        footer_line.fill.solid()
        footer_line.fill.fore_color.rgb = BORDER_COLOR
        footer_line.line.color.rgb = BORDER_COLOR

        # Footer Text
        foot_box = slide.shapes.add_textbox(Inches(0.8), Inches(6.95), Inches(11.733), Inches(0.4))
        tf_foot = foot_box.text_frame
        p_foot = tf_foot.paragraphs[0]
        p_foot.text = "Student Record Management System (SRMS) | Shubham Gupta (45) & Swapnil Yadav (46)"
        p_foot.font.size = Pt(9)
        p_foot.font.color.rgb = MUTED_TEXT

    def add_card(slide, left, top, width, height, title, items, bg_color=CARD_BG, title_color=PRIMARY, border_color=BORDER_COLOR):
        # Card shape
        card = slide.shapes.add_shape(MSO_SHAPE.ROUNDED_RECTANGLE, left, top, width, height)
        card.fill.solid()
        card.fill.fore_color.rgb = bg_color
        card.line.color.rgb = border_color
        card.line.width = Pt(1)

        # Header bar inside card
        header_box = slide.shapes.add_textbox(left + Inches(0.2), top + Inches(0.15), width - Inches(0.4), Inches(0.4))
        tf_h = header_box.text_frame
        tf_h.word_wrap = True
        p_h = tf_h.paragraphs[0]
        p_h.text = title
        p_h.font.size = Pt(14)
        p_h.font.bold = True
        p_h.font.color.rgb = title_color

        # Body text
        body_box = slide.shapes.add_textbox(left + Inches(0.2), top + Inches(0.6), width - Inches(0.4), height - Inches(0.75))
        tf_b = body_box.text_frame
        tf_b.word_wrap = True
        
        for idx, item in enumerate(items):
            if idx == 0:
                p = tf_b.paragraphs[0]
            else:
                p = tf_b.add_paragraph()
            p.text = "• " + item if isinstance(item, str) and not item.startswith("  ") else item
            p.font.size = Pt(11)
            p.font.color.rgb = DARK_TEXT
            p.space_after = Pt(6)

    # -------------------------------------------------------------
    # SLIDE 1: TITLE SLIDE
    # -------------------------------------------------------------
    s1 = prs.slides.add_slide(blank_layout)
    # Background
    bg1 = s1.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0), Inches(0), Inches(13.333), Inches(7.5))
    bg1.fill.solid()
    bg1.fill.fore_color.rgb = PRIMARY
    bg1.line.color.rgb = PRIMARY

    # Decorative top bar
    bar1 = s1.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0), Inches(0), Inches(13.333), Inches(0.2))
    bar1.fill.solid()
    bar1.fill.fore_color.rgb = ACCENT
    bar1.line.color.rgb = ACCENT

    # Main Title
    t_box1 = s1.shapes.add_textbox(Inches(1.0), Inches(1.8), Inches(11.333), Inches(2.2))
    tf1 = t_box1.text_frame
    tf1.word_wrap = True
    p1 = tf1.paragraphs[0]
    p1.text = "STUDENT RECORD MANAGEMENT SYSTEM"
    p1.font.size = Pt(36)
    p1.font.bold = True
    p1.font.color.rgb = WHITE
    p1.alignment = PP_ALIGN.CENTER

    p1_sub = tf1.add_paragraph()
    p1_sub.text = "Detailed Design, Class Descriptions, Workflows, Database Schema & C++ Implementation"
    p1_sub.font.size = Pt(16)
    p1_sub.font.color.rgb = ACCENT
    p1_sub.alignment = PP_ALIGN.CENTER
    p1_sub.space_before = Pt(12)

    # Info card on cover
    cover_card = s1.shapes.add_shape(MSO_SHAPE.ROUNDED_RECTANGLE, Inches(2.2), Inches(4.3), Inches(8.933), Inches(2.3))
    cover_card.fill.solid()
    cover_card.fill.fore_color.rgb = RGBColor(15, 52, 96)
    cover_card.line.color.rgb = ACCENT
    cover_card.line.width = Pt(1.5)

    c_box = s1.shapes.add_textbox(Inches(2.5), Inches(4.5), Inches(8.333), Inches(1.9))
    tf_c = c_box.text_frame
    tf_c.word_wrap = True

    p_team_h = tf_c.paragraphs[0]
    p_team_h.text = "ACADEMIC PROJECT EVALUATION PRESENTATION"
    p_team_h.font.size = Pt(12)
    p_team_h.font.bold = True
    p_team_h.font.color.rgb = ACCENT
    p_team_h.alignment = PP_ALIGN.CENTER

    p_names = tf_c.add_paragraph()
    p_names.text = "Prepared by: Shubham Gupta (Roll No: 45)  &  Swapnil Yadav (Roll No: 46)"
    p_names.font.size = Pt(14)
    p_names.font.bold = True
    p_names.font.color.rgb = WHITE
    p_names.alignment = PP_ALIGN.CENTER
    p_names.space_before = Pt(8)

    p_tags = tf_c.add_paragraph()
    p_tags.text = "Language: C++ (C++17) | Architecture: 3-Tier OOP | Git Milestones: v1.0-mvp, v2.0-enhanced, v3.0-final"
    p_tags.font.size = Pt(11)
    p_tags.font.color.rgb = RGBColor(203, 213, 225)
    p_tags.alignment = PP_ALIGN.CENTER
    p_tags.space_before = Pt(6)

    # -------------------------------------------------------------
    # SLIDE 2: AGENDA / TABLE OF CONTENTS
    # -------------------------------------------------------------
    s2 = prs.slides.add_slide(blank_layout)
    add_header(s2, "Presentation Agenda & Overview", "ROADMAP")

    agenda_items_left = [
        "1. Introduction & Background",
        "2. Problem Statement & Manual Records Flaws",
        "3. Project Objectives & Scope",
        "4. System Architecture (3-Tier Model)",
        "5. Object-Oriented Class Design (OOP)",
        "6. Relational Database & Entity Model",
        "7. System Workflows & Lifecycle"
    ]
    agenda_items_right = [
        "8. Data Validation & Security Standards",
        "9. Quality Assurance & Testing (TC-01 to TC-07)",
        "10. Project Evaluation 1: MVP Release (v1.0-mvp)",
        "11. Project Evaluation 2: Version 2 (v2.0-enhanced)",
        "12. Project Evaluation 3: Final System (v3.0-final)",
        "13. Deliverables & Demonstration Highlights",
        "14. Limitations, Future Scope & Conclusion"
    ]
    add_card(s2, Inches(0.8), Inches(1.6), Inches(5.6), Inches(5.0), "Core Theoretical Foundations", agenda_items_left, title_color=SECONDARY)
    add_card(s2, Inches(6.8), Inches(1.6), Inches(5.6), Inches(5.0), "Technical Execution & Milestones", agenda_items_right, title_color=SECONDARY)

    # -------------------------------------------------------------
    # SLIDE 3: INTRODUCTION & PROBLEM STATEMENT
    # -------------------------------------------------------------
    s3 = prs.slides.add_slide(blank_layout)
    add_header(s3, "Background & Problem Statement", "PROJECT FOUNDATIONS")

    intro_points = [
        "The Student Record Management System (SRMS) is a centralized software platform designed to manage institutional student data efficiently.",
        "Educational institutions manage high volumes of student profiles, enrollments, course catalogs, academic assessments, and daily attendance.",
        "Transitioning from manual paperwork to a structured digital application provides reliable integrity, rapid retrieval, and automated calculations."
    ]
    prob_points = [
        "Manual Register Fragmentation: Student records are scattered across physical ledgers, spreadsheets, and departmental paper files.",
        "Inefficient Searching: Locating individual student history or course lists requires laborious manual searching.",
        "Calculation & Transcript Errors: Manual aggregation of subject marks, percentages, and GPA calculation leads to frequent arithmetic errors.",
        "Security & Audit Deficiencies: Paper files lack role-based access control, encryption, backup mechanisms, and historical audit preservation."
    ]
    add_card(s3, Inches(0.8), Inches(1.6), Inches(5.6), Inches(5.0), "System Background & Need", intro_points)
    add_card(s3, Inches(6.8), Inches(1.6), Inches(5.6), Inches(5.0), "The Problem with Manual Records", prob_points, title_color=RGBColor(183, 28, 28))

    # -------------------------------------------------------------
    # SLIDE 4: OBJECTIVES AND SCOPE
    # -------------------------------------------------------------
    s4 = prs.slides.add_slide(blank_layout)
    add_header(s4, "Project Objectives & Scope Boundaries", "PROJECT SPECIFICATION")

    obj_points = [
        "Centralized Digital Repository: Maintain student profiles, contact details, and courses in a consistent format.",
        "Comprehensive CRUD Operations: Implement functions to Create, Read, Update, and Soft-Deactivate student records.",
        "Referential Academic Tracking: Link student IDs to courses across academic years and terms.",
        "Automated Academic Computation: Calculate cumulative percentages and assign official letter grades (A+, A, B, C, D, F).",
        "Attendance Management: Record daily attendance (Present/Absent/Late) and compute percentage rates.",
        "Role-Based Access Control: Distinguish Administrator actions from Faculty/Staff operations."
    ]
    scope_points = [
        "In Scope (Delivered):",
        "  - Complete Student Demographic Lifecycle Management",
        "  - Course Directory & Department Allocation",
        "  - Enrollment Registry & Semester Linkage",
        "  - Assessment Marks Recording & Validation",
        "  - Daily Attendance Tracking & Percentage Statistics",
        "  - Academic Transcript & Roster File Export to Disk",
        "Out of Scope (Future Institutional Extensions):",
        "  - Live online payment gateway for fee collection",
        "  - Automated SMS/Email notification microservices",
        "  - Multi-tenant cloud-hosted web portal"
    ]
    add_card(s4, Inches(0.8), Inches(1.6), Inches(5.6), Inches(5.0), "Core Project Objectives", obj_points)
    add_card(s4, Inches(6.8), Inches(1.6), Inches(5.6), Inches(5.0), "Operational Scope", scope_points, title_color=SECONDARY)

    # -------------------------------------------------------------
    # SLIDE 5: 3-TIER SYSTEM ARCHITECTURE
    # -------------------------------------------------------------
    s5 = prs.slides.add_slide(blank_layout)
    add_header(s5, "3-Tier System Architecture Design", "TECHNICAL ARCHITECTURE")

    tier1 = [
        "Console Interactive UI (`main.cpp`)",
        "Session Login & Gateway Screens",
        "Student Profile Forms & Roster Views",
        "Advanced Multi-Criteria Search Filters",
        "Marks Entry & Assessment Views",
        "Academic Transcript Visualizer",
        "Clear Feedback & Error Displays"
    ]
    tier2 = [
        "Authentication Service (`User` / RBAC)",
        "Student Management Service (`Student`)",
        "Course Catalog Service (`Course`)",
        "Enrollment Engine (`Enrollment`)",
        "Marks & Scoring Engine (`Mark`)",
        "Attendance Service (`Attendance`)",
        "Report & Transcript Generator (`ReportService`)"
    ]
    tier3 = [
        "Relational Flat-File Persistent Store (`data/`)",
        "`users.txt` (Hashed credentials & roles)",
        "`students.txt` (Demographics & status)",
        "`courses.txt` (Codes, names, departments)",
        "`enrollments.txt` (Foreign key links)",
        "`marks.txt` (Subject scores & boundaries)",
        "`attendance.txt` (Daily attendance records)"
    ]
    add_card(s5, Inches(0.8), Inches(1.6), Inches(3.64), Inches(5.0), "1. Presentation Layer", tier1, title_color=SECONDARY)
    add_card(s5, Inches(4.84), Inches(1.6), Inches(3.64), Inches(5.0), "2. Application Logic Layer", tier2, title_color=PRIMARY)
    add_card(s5, Inches(8.88), Inches(1.6), Inches(3.64), Inches(5.0), "3. Data Persistence Layer", tier3, title_color=SECONDARY)

    # -------------------------------------------------------------
    # SLIDE 6: OBJECT-ORIENTED CLASS DESIGN
    # -------------------------------------------------------------
    s6 = prs.slides.add_slide(blank_layout)
    add_header(s6, "Object-Oriented Class Descriptions (C++)", "DETAILED DESIGN")

    c1 = [
        "User: userId, username, passwordHash, role, active",
        "  - login(), logout(), hasPermission()",
        "Student: studentId, fullName, dob, email, phone, address, courseId, status",
        "  - validateDetails(), updateProfile(), getSummary()",
        "Course: courseId, courseName, department, durationYears",
        "  - validateCourse(), updateCourse()"
    ]
    c2 = [
        "Enrollment: enrollmentId, studentId, courseId, academicYear, semester, enrollmentDate",
        "  - enrollStudent(), changeCourse(), getEnrollmentDetails()",
        "Mark: markId, studentId, subject, assessment, score, maxScore",
        "  - validateScore(), calculatePercentage()",
        "Attendance: attendanceId, studentId, date, status (Present/Absent/Late)",
        "  - validate(), serialize(), deserialize()"
    ]
    c3 = [
        "StudentRepository: Core relational persistence & memory cache",
        "  - add(), findById(), search(), update(), deactivate(), remove(), addCourse(), addEnrollment(), addMark(), addAttendance()",
        "ReportService: Academic reporting & disk export",
        "  - studentList(), marksSummary(), calculateGrade(), exportReport()"
    ]
    add_card(s6, Inches(0.8), Inches(1.6), Inches(3.64), Inches(5.0), "Core Entities", c1)
    add_card(s6, Inches(4.84), Inches(1.6), Inches(3.64), Inches(5.0), "Academic Relationships", c2)
    add_card(s6, Inches(8.88), Inches(1.6), Inches(3.64), Inches(5.0), "Services & Data Layer", c3)

    # -------------------------------------------------------------
    # SLIDE 7: DATABASE SCHEMA & RELATIONAL MODEL
    # -------------------------------------------------------------
    s7 = prs.slides.add_slide(blank_layout)
    add_header(s7, "Database Design & Relational Schema", "DATA MODEL")

    tables1 = [
        "USERS (Authentication & RBAC):",
        "  - user_id (PK), username (UK), password_hash, role, active",
        "STUDENT (Core Demographic Entity):",
        "  - student_id (PK), full_name, dob, email (UK), phone, address, course_id (FK), status",
        "COURSE (Academic Catalog):",
        "  - course_id (PK), course_name, department, duration"
    ]
    tables2 = [
        "ENROLLMENT (Student-Course Term Link):",
        "  - enrollment_id (PK), student_id (FK), course_id (FK), academic_year, semester, date",
        "MARKS (Assessment Scores):",
        "  - mark_id (PK), student_id (FK), subject, assessment, score, max_score",
        "  - Constraint: 0 <= score <= max_score",
        "ATTENDANCE (Class Participation):",
        "  - attendance_id (PK), student_id (FK), date, status ('Present'/'Absent'/'Late')"
    ]
    add_card(s7, Inches(0.8), Inches(1.6), Inches(5.6), Inches(5.0), "Master Tables", tables1)
    add_card(s7, Inches(6.8), Inches(1.6), Inches(5.6), Inches(5.0), "Transaction & Assessment Tables", tables2)

    # -------------------------------------------------------------
    # SLIDE 8: SYSTEM WORKFLOWS & LIFECYCLE
    # -------------------------------------------------------------
    s8 = prs.slides.add_slide(blank_layout)
    add_header(s8, "Operational Workflows & Sequences", "PROCESS FLOW")

    wf1 = [
        "1. System Launch & Banner Display",
        "2. Credential Verification (Username + Hashed Password)",
        "3. Session Authorization (Admin vs. Faculty/Staff)",
        "4. Interactive Dashboard Routing",
        "5. Form Data Entry & Defensive Validation",
        "6. Relational Persistence to Disk",
        "7. User Feedback & Safe Logout"
    ]
    wf2 = [
        "Add Student Sequence:",
        "  - Form input -> Check non-empty name & ID -> Validate email & phone -> Check duplicate ID in database -> Persist record -> Display confirmation.",
        "Marks Entry Sequence:",
        "  - Select student -> Verify student exists (FK check) -> Enter subject & assessment -> Validate 0 <= score <= maxScore -> Persist & compute %.",
        "Search & Update Sequence:",
        "  - Query keyword -> Match ID/Name/Course -> Load record -> Modify phone/address -> Validate -> Update disk cache."
    ]
    add_card(s8, Inches(0.8), Inches(1.6), Inches(5.6), Inches(5.0), "Main System Lifecycle", wf1)
    add_card(s8, Inches(6.8), Inches(1.6), Inches(5.6), Inches(5.0), "Key Operational Sub-Workflows", wf2, title_color=SECONDARY)

    # -------------------------------------------------------------
    # SLIDE 9: DATA VALIDATION & SECURITY STANDARDS
    # -------------------------------------------------------------
    s9 = prs.slides.add_slide(blank_layout)
    add_header(s9, "Data Validation & Defensive Security", "INTEGRITY & SAFETY")

    val_points = [
        "Duplicate Identifier Prevention: Every insert validates that studentId is globally unique (TC-04).",
        "Field Sanity Checks: Required fields cannot be empty; white-spaces trimmed automatically.",
        "Format Verification: Email patterns must contain '@' and domain; phone numbers validated for 7 to 15 digits.",
        "Score Boundary Constraint: Strict enforcement that marks obtained cannot exceed maximum allowed marks (TC-06)."
    ]
    sec_points = [
        "Cryptographic Password Hashing: User credentials stored as salted hashes; plaintext passwords never written to disk.",
        "Role-Based Access Control (RBAC): Administrative functions (user creation, account toggles, hard deletion) restricted from staff.",
        "Soft Deactivation Policy: Student profiles set to 'Inactive' instead of hard deletion to retain transcript history (Section 6.3).",
        "Defensive File IO: Clean serialization preventing data corruption across concurrent sessions."
    ]
    add_card(s9, Inches(0.8), Inches(1.6), Inches(5.6), Inches(5.0), "Input Validation Rules", val_points)
    add_card(s9, Inches(6.8), Inches(1.6), Inches(5.6), Inches(5.0), "Security & Audit Standards", sec_points, title_color=SECONDARY)

    # -------------------------------------------------------------
    # SLIDE 10: AUTOMATED TEST SUITE (TC-01 TO TC-07)
    # -------------------------------------------------------------
    s10 = prs.slides.add_slide(blank_layout)
    add_header(s10, "Quality Assurance: Formal Testing Matrix", "VERIFICATION PLAN")

    tc_left = [
        "TC-01: Valid Login",
        "  - Input: admin / admin123 -> Result: Dashboard opens [PASS]",
        "TC-02: Invalid Login",
        "  - Input: wrong password -> Result: Access denied message [PASS]",
        "TC-03: Add Student (Valid Details)",
        "  - Input: New valid student record -> Result: Saved & searchable [PASS]",
        "TC-04: Duplicate Student ID",
        "  - Input: Existing ID 'S101' -> Result: Blocked with explanation [PASS]"
    ]
    tc_right = [
        "TC-05: Update Profile",
        "  - Input: Modify phone/address -> Result: Updated in database [PASS]",
        "TC-06: Invalid Marks Entry",
        "  - Input: Score 110 / 100 -> Result: Rejected with range error [PASS]",
        "TC-07: Search Missing Student ID",
        "  - Input: Unknown ID 'S9999' -> Result: No matching record message [PASS]",
        "AUTOMATED TEST EXECUTION SUMMARY:",
        "  - 7 out of 7 Test Cases PASSED (100% SUCCESS RATE)"
    ]
    add_card(s10, Inches(0.8), Inches(1.6), Inches(5.6), Inches(5.0), "Test Cases: Auth & Demographics", tc_left)
    add_card(s10, Inches(6.8), Inches(1.6), Inches(5.6), Inches(5.0), "Test Cases: Updates, Marks & Lookup", tc_right, title_color=SUCCESS)

    # -------------------------------------------------------------
    # SLIDE 11: EVALUATION 1 MILESTONE (MVP)
    # -------------------------------------------------------------
    s11 = prs.slides.add_slide(blank_layout)
    add_header(s11, "Project Evaluation 1: MVP Release", "MILESTONE 1 (TAG: v1.0-mvp)")

    e1_points = [
        "Scope & Purpose: First Cut / Minimum Viable Product (MVP).",
        "Domain Model Implementation: `User`, `Student`, and `Course` classes.",
        "Authentication Engine: Admin and Staff login with credential validation.",
        "Basic Student CRUD: Add student, view all roster records, search student by ID.",
        "Flat-File Persistence: `data/users.txt`, `data/students.txt`, `data/courses.txt`.",
        "Validation Verified: Non-empty inputs and duplicate ID prevention (TC-04)."
    ]
    e1_git = [
        "Git Milestone Tag: `v1.0-mvp`",
        "Commit Hash: `d2f24e7`",
        "Commit Title: 'Evaluation 1: Demo of First cut / MVP version'",
        "Generated Artifacts: `Evaluation_1_Code_Report.pdf` (282 KB)",
        "Rubric Alignment: Demo of First cut / MVP; code pushed to repository (Marks: 4/4)."
    ]
    add_card(s11, Inches(0.8), Inches(1.6), Inches(5.6), Inches(5.0), "Technical Features Delivered", e1_points)
    add_card(s11, Inches(6.8), Inches(1.6), Inches(5.6), Inches(5.0), "Git Evidence & Artifacts", e1_git, title_color=SECONDARY)

    # -------------------------------------------------------------
    # SLIDE 12: EVALUATION 2 MILESTONE (VERSION 2)
    # -------------------------------------------------------------
    s12 = prs.slides.add_slide(blank_layout)
    add_header(s12, "Project Evaluation 2: Version 2 Release", "MILESTONE 2 (TAG: v2.0-enhanced)")

    e2_points = [
        "Scope & Purpose: Enhanced Feature Set & Academic Relationships.",
        "Enrollment Management: `Enrollment.h/cpp` linking student IDs to courses, academic years, and semesters.",
        "Marks Assessment Engine: `Mark.h/cpp` recording subject scores with automated percentage calculation.",
        "Advanced Multi-Criteria Search: Instant keyword lookup across ID, Name, and Course Code.",
        "Profile Updating: Edit student phone and address with live persistence (TC-05).",
        "Marks Boundary Enforcement: Rejects scores exceeding maximum allowed (TC-06)."
    ]
    e2_git = [
        "Git Milestone Tag: `v2.0-enhanced`",
        "Commit Hash: `c7ca262`",
        "Commit Title: 'Evaluation 2: Version 2 - Enrolment, Marks, Search & Validation'",
        "Generated Artifacts: `Evaluation_2_Code_Report.pdf` (246 KB)",
        "Rubric Alignment: Demo of version 2; code pushed to repository (Marks: 4/4)."
    ]
    add_card(s12, Inches(0.8), Inches(1.6), Inches(5.6), Inches(5.0), "Technical Features Delivered", e2_points)
    add_card(s12, Inches(6.8), Inches(1.6), Inches(5.6), Inches(5.0), "Git Evidence & Artifacts", e2_git, title_color=SECONDARY)

    # -------------------------------------------------------------
    # SLIDE 13: EVALUATION 3 MILESTONE (FINAL SYSTEM)
    # -------------------------------------------------------------
    s13 = prs.slides.add_slide(blank_layout)
    add_header(s13, "Project Evaluation 3: Final Production System", "MILESTONE 3 (TAG: v3.0-final)")

    e3_points = [
        "Full Role-Based Access Control: Admin user management (add account, toggle active status).",
        "Academic Report & Transcript Service: `ReportService.h/cpp` compiling cumulative scores, percentages, and official letter grades (A+, A, B, C, D, F).",
        "Attendance Tracking Module: `Attendance.h/cpp` logging daily status (Present/Absent/Late) & percentage.",
        "Soft Deactivation & Hard Delete: Soft-delete retains transcript history; Admin hard-delete option.",
        "Automated Test Suite: `tests/test_srms.cpp` validating TC-01 to TC-07 (100% PASS).",
        "One-Click Launchers: `START_SYSTEM.bat` and `RUN_TESTS.bat`."
    ]
    e3_git = [
        "Git Milestone Tag: `v3.0-final`",
        "Commit Hash: `3a40b1b` (Latest: `3f940ec`)",
        "Commit Title: 'Evaluation 3: Version 3 - Final Complete System'",
        "Generated Artifacts: `Evaluation_3_Code_Report.pdf` (265 KB)",
        "Rubric Alignment: Demo of version 3; full codebase pushed to repository (Marks: 4/4)."
    ]
    add_card(s13, Inches(0.8), Inches(1.6), Inches(5.6), Inches(5.0), "Technical Features Delivered", e3_points)
    add_card(s13, Inches(6.8), Inches(1.6), Inches(5.6), Inches(5.0), "Git Evidence & Artifacts", e3_git, title_color=SUCCESS)

    # -------------------------------------------------------------
    # SLIDE 14: OUTPUTS & DEMONSTRATION HIGHLIGHTS
    # -------------------------------------------------------------
    s14 = prs.slides.add_slide(blank_layout)
    add_header(s14, "System Demonstration & Exported Reports", "OUTPUTS")

    out1 = [
        "Official Student Academic Transcript:",
        "  - Student ID: S101 | Name: Shubham Gupta | Course: CS101",
        "  - Data Structures: 92.5 / 100 (92.5%)",
        "  - Database Systems: 88.0 / 100 (88.0%)",
        "  - Computer Networks: 46.0 / 50 (92.0%)",
        "  - Cumulative Score: 226.5 / 250 (90.6%)",
        "  - Official Grade Awarded: A+ (Outstanding)",
        "  - Exported to: `reports/transcript_S101.txt`"
    ]
    out2 = [
        "Class Roster & Demographic Listing:",
        "  - Student rosters filtered by course or active status.",
        "  - Exported to: `reports/student_roster.txt`.",
        "Attendance Statistics:",
        "  - Student S101 Attendance Rate: 100.0% (3/3 Present).",
        "  - Student S102 Attendance Rate: 66.7% (2 Present, 1 Absent).",
        "One-Click Launchers:",
        "  - `START_SYSTEM.bat`: Launches interactive application.",
        "  - `RUN_TESTS.bat`: Launches automated unit test verification."
    ]
    add_card(s14, Inches(0.8), Inches(1.6), Inches(5.6), Inches(5.0), "Academic Transcript Output", out1)
    add_card(s14, Inches(6.8), Inches(1.6), Inches(5.6), Inches(5.0), "Roster & Attendance Outputs", out2, title_color=SECONDARY)

    # -------------------------------------------------------------
    # SLIDE 15: LIMITATIONS & FUTURE ENHANCEMENTS
    # -------------------------------------------------------------
    s15 = prs.slides.add_slide(blank_layout)
    add_header(s15, "Limitations & Future Roadmap", "FUTURE ENHANCEMENTS")

    lim_points = [
        "Current System Boundaries (Section 12 of Report):",
        "  - Standalone Application: Designed as a local C++ command-line application.",
        "  - Storage Architecture: Employs persistent structured flat-files rather than client-server SQL servers.",
        "  - User Interface: Console-based terminal UI without graphic browser windows."
    ]
    fut_points = [
        "Planned Future Enhancements:",
        "  - Web & Mobile Frontends: Modern responsive web interface using React / Vue with RESTful APIs.",
        "  - Enterprise SQL Migration: Migration from flat-file storage to MySQL / PostgreSQL clusters.",
        "  - Student & Parent Portals: Dedicated self-service portals for viewing grades and attendance.",
        "  - Automated Communication: SMS and email notifications for low attendance or report card generation.",
        "  - Fee Payment & Financial Tracking: Integrated payment gateway for tuition and campus dues."
    ]
    add_card(s15, Inches(0.8), Inches(1.6), Inches(5.6), Inches(5.0), "Current Limitations", lim_points)
    add_card(s15, Inches(6.8), Inches(1.6), Inches(5.6), Inches(5.0), "Future Institutional Extensions", fut_points, title_color=SECONDARY)

    # -------------------------------------------------------------
    # SLIDE 16: CONCLUSION & THANK YOU
    # -------------------------------------------------------------
    s16 = prs.slides.add_slide(blank_layout)
    bg16 = s16.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0), Inches(0), Inches(13.333), Inches(7.5))
    bg16.fill.solid()
    bg16.fill.fore_color.rgb = PRIMARY
    bg16.line.color.rgb = PRIMARY

    bar16 = s16.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0), Inches(0), Inches(13.333), Inches(0.2))
    bar16.fill.solid()
    bar16.fill.fore_color.rgb = ACCENT
    bar16.line.color.rgb = ACCENT

    t_box16 = s16.shapes.add_textbox(Inches(1.0), Inches(1.5), Inches(11.333), Inches(1.5))
    tf16 = t_box16.text_frame
    p16 = tf16.paragraphs[0]
    p16.text = "PROJECT CONCLUSION & SUMMARY"
    p16.font.size = Pt(32)
    p16.font.bold = True
    p16.font.color.rgb = WHITE
    p16.alignment = PP_ALIGN.CENTER

    end_card = s16.shapes.add_shape(MSO_SHAPE.ROUNDED_RECTANGLE, Inches(1.5), Inches(3.0), Inches(10.333), Inches(3.5))
    end_card.fill.solid()
    end_card.fill.fore_color.rgb = RGBColor(15, 52, 96)
    end_card.line.color.rgb = ACCENT
    end_card.line.width = Pt(1.5)

    ec_box = s16.shapes.add_textbox(Inches(1.8), Inches(3.2), Inches(9.733), Inches(3.1))
    tf_ec = ec_box.text_frame
    tf_ec.word_wrap = True

    p_sum1 = tf_ec.paragraphs[0]
    p_sum1.text = "Key Project Achievements:"
    p_sum1.font.size = Pt(14)
    p_sum1.font.bold = True
    p_sum1.font.color.rgb = ACCENT

    p_sum2 = tf_ec.add_paragraph()
    p_sum2.text = "• Successfully delivered 100% of deliverables from academic proposal & design specification."
    p_sum2.font.size = Pt(12)
    p_sum2.font.color.rgb = WHITE
    p_sum2.space_before = Pt(4)

    p_sum3 = tf_ec.add_paragraph()
    p_sum3.text = "• All three milestones (Evaluation 1, Evaluation 2, Evaluation 3) tagged & pushed to GitHub."
    p_sum3.font.size = Pt(12)
    p_sum3.font.color.rgb = WHITE

    p_sum4 = tf_ec.add_paragraph()
    p_sum4.text = "• Complete test suite with 100% pass rate validating all 7 official test scenarios (TC-01 to TC-07)."
    p_sum4.font.size = Pt(12)
    p_sum4.font.color.rgb = WHITE

    p_sum5 = tf_ec.add_paragraph()
    p_sum5.text = "• Generated official academic PDF reports for all three evaluation submissions."
    p_sum5.font.size = Pt(12)
    p_sum5.font.color.rgb = WHITE

    p_thanks = tf_ec.add_paragraph()
    p_thanks.text = "THANK YOU! QUESTIONS & DISCUSSION WELCOME"
    p_thanks.font.size = Pt(16)
    p_thanks.font.bold = True
    p_thanks.font.color.rgb = ACCENT
    p_thanks.alignment = PP_ALIGN.CENTER
    p_thanks.space_before = Pt(14)

    # Save presentation
    output_filename = "Student_Record_Management_System_Presentation.pptx"
    prs.save(output_filename)
    print(f"Presentation saved successfully as '{output_filename}'")

if __name__ == "__main__":
    create_presentation()
