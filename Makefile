CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Iinclude

SRCS = src/User.cpp src/Course.cpp src/Student.cpp src/Enrollment.cpp src/Mark.cpp src/Attendance.cpp src/ReportService.cpp src/StudentRepository.cpp
MAIN = src/main.cpp
TEST_SRC = tests/test_srms.cpp

TARGET = srms.exe
TEST_TARGET = test_suite.exe

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(MAIN) $(SRCS)
	$(CXX) $(CXXFLAGS) $(MAIN) $(SRCS) -o $(TARGET)

$(TEST_TARGET): $(TEST_SRC) $(SRCS)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(SRCS) -o $(TEST_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o
