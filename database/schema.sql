-- =============================================================================
-- STUDENT RECORD MANAGEMENT SYSTEM (SRMS) - RELATIONAL DATABASE SCHEMA
-- Authors: Shubham Gupta (Roll No: 45) & Swapnil Yadav (Roll No: 46)
-- Deliverable: Database Design & Starter Schema (MySQL / SQLite / PostgreSQL)
-- =============================================================================

-- Drop tables in reverse order of foreign key dependencies
DROP TABLE IF EXISTS ATTENDANCE;
DROP TABLE IF EXISTS MARKS;
DROP TABLE IF EXISTS ENROLLMENT;
DROP TABLE IF EXISTS COURSE;
DROP TABLE IF EXISTS STUDENT;
DROP TABLE IF EXISTS USERS;

-- 1. USERS Table (Access Control & Authentication)
CREATE TABLE USERS (
    user_id VARCHAR(20) PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password_hash VARCHAR(128) NOT NULL,
    role VARCHAR(20) NOT NULL CHECK (role IN ('Admin', 'Staff')),
    active BOOLEAN DEFAULT TRUE
);

-- 2. STUDENT Table (Core Student Entity)
CREATE TABLE STUDENT (
    student_id VARCHAR(20) PRIMARY KEY,
    full_name VARCHAR(100) NOT NULL,
    dob DATE,
    email VARCHAR(120) UNIQUE,
    phone VARCHAR(20),
    address VARCHAR(250),
    course_id VARCHAR(20),
    status VARCHAR(20) DEFAULT 'Active' CHECK (status IN ('Active', 'Inactive'))
);

-- 3. COURSE Table (Program & Degree Catalog)
CREATE TABLE COURSE (
    course_id VARCHAR(20) PRIMARY KEY,
    course_name VARCHAR(100) NOT NULL,
    department VARCHAR(50) NOT NULL,
    duration INT NOT NULL CHECK (duration BETWEEN 1 AND 6)
);

-- 4. ENROLLMENT Table (Student to Course / Term Link)
CREATE TABLE ENROLLMENT (
    enrollment_id VARCHAR(20) PRIMARY KEY,
    student_id VARCHAR(20) NOT NULL,
    course_id VARCHAR(20) NOT NULL,
    academic_year VARCHAR(20) NOT NULL,
    semester VARCHAR(20) NOT NULL,
    enrollment_date DATE NOT NULL,
    FOREIGN KEY (student_id) REFERENCES STUDENT(student_id) ON DELETE CASCADE,
    FOREIGN KEY (course_id) REFERENCES COURSE(course_id) ON DELETE RESTRICT
);

-- 5. MARKS Table (Assessment Results)
CREATE TABLE MARKS (
    mark_id VARCHAR(20) PRIMARY KEY,
    student_id VARCHAR(20) NOT NULL,
    subject VARCHAR(100) NOT NULL,
    assessment VARCHAR(50) NOT NULL,
    score DECIMAL(5,2) NOT NULL,
    max_score DECIMAL(5,2) NOT NULL,
    CONSTRAINT chk_score_range CHECK (score >= 0 AND score <= max_score),
    FOREIGN KEY (student_id) REFERENCES STUDENT(student_id) ON DELETE CASCADE
);

-- 6. ATTENDANCE Table (Daily Attendance Tracking)
CREATE TABLE ATTENDANCE (
    attendance_id VARCHAR(20) PRIMARY KEY,
    student_id VARCHAR(20) NOT NULL,
    attendance_date DATE NOT NULL,
    status VARCHAR(10) NOT NULL CHECK (status IN ('Present', 'Absent', 'Late')),
    FOREIGN KEY (student_id) REFERENCES STUDENT(student_id) ON DELETE CASCADE
);

-- =============================================================================
-- SEED DATA INSERTION (Initial Population)
-- =============================================================================

INSERT INTO USERS (user_id, username, password_hash, role, active) VALUES
('U001', 'admin', 'sha256_00000000admin123', 'Admin', TRUE),
('U002', 'staff', 'sha256_00000000staff123', 'Staff', TRUE);

INSERT INTO COURSE (course_id, course_name, department, duration) VALUES
('CS101', 'Computer Science & Engineering', 'CSE', 4),
('IT102', 'Information Technology', 'IT', 4),
('EC103', 'Electronics & Communication', 'ECE', 4),
('ME104', 'Mechanical Engineering', 'MECH', 4);

INSERT INTO STUDENT (student_id, full_name, dob, email, phone, address, course_id, status) VALUES
('S101', 'Shubham Gupta', '2003-05-14', 'shubham@univ.edu', '9876543210', 'Block A, Campus Hostel', 'CS101', 'Active'),
('S102', 'Swapnil Yadav', '2003-08-22', 'swapnil@univ.edu', '9876543211', 'Block B, Campus Hostel', 'CS101', 'Active'),
('S103', 'Aarav Sharma', '2003-01-10', 'aarav@univ.edu', '9876543212', 'Green Park, City', 'IT102', 'Active');

INSERT INTO ENROLLMENT (enrollment_id, student_id, course_id, academic_year, semester, enrollment_date) VALUES
('E001', 'S101', 'CS101', '2024-2025', 'Semester 5', '2024-07-15'),
('E002', 'S102', 'CS101', '2024-2025', 'Semester 5', '2024-07-15'),
('E003', 'S103', 'IT102', '2024-2025', 'Semester 5', '2024-07-16');

INSERT INTO MARKS (mark_id, student_id, subject, assessment, score, max_score) VALUES
('M001', 'S101', 'Data Structures & Algorithms', 'Final Exam', 92.50, 100.00),
('M002', 'S101', 'Database Management Systems', 'Final Exam', 88.00, 100.00),
('M003', 'S101', 'Computer Networks', 'Mid-Term', 46.00, 50.00),
('M004', 'S102', 'Data Structures & Algorithms', 'Final Exam', 89.00, 100.00),
('M005', 'S102', 'Database Management Systems', 'Final Exam', 91.50, 100.00),
('M006', 'S102', 'Operating Systems', 'Mid-Term', 44.00, 50.00),
('M007', 'S103', 'Web Development', 'Final Exam', 85.00, 100.00),
('M008', 'S103', 'Software Engineering', 'Final Exam', 82.00, 100.00);

INSERT INTO ATTENDANCE (attendance_id, student_id, attendance_date, status) VALUES
('A001', 'S101', '2024-11-01', 'Present'),
('A002', 'S101', '2024-11-02', 'Present'),
('A003', 'S101', '2024-11-03', 'Present'),
('A004', 'S102', '2024-11-01', 'Present'),
('A005', 'S102', '2024-11-02', 'Absent'),
('A006', 'S102', '2024-11-03', 'Present');
