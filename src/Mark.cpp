#include "../include/Mark.h"
#include <sstream>

Mark::Mark() : markId(""), studentId(""), subject(""), assessment(""), score(0.0), maxScore(100.0) {}

Mark::Mark(const std::string& mId, const std::string& sId, const std::string& sub,
           const std::string& assess, double sc, double maxSc)
    : markId(mId), studentId(sId), subject(sub), assessment(assess), score(sc), maxScore(maxSc) {}

std::string Mark::getMarkId() const { return markId; }
std::string Mark::getStudentId() const { return studentId; }
std::string Mark::getSubject() const { return subject; }
std::string Mark::getAssessment() const { return assessment; }
double Mark::getScore() const { return score; }
double Mark::getMaxScore() const { return maxScore; }

bool Mark::validateScore(std::string& errorMessage) const {
    if (Utils::trim(markId).empty()) {
        errorMessage = "Validation Error: Mark ID cannot be empty.";
        return false;
    }
    if (Utils::trim(studentId).empty()) {
        errorMessage = "Validation Error: Student ID cannot be empty.";
        return false;
    }
    if (Utils::trim(subject).empty()) {
        errorMessage = "Validation Error: Subject cannot be empty.";
        return false;
    }
    if (maxScore <= 0.0) {
        errorMessage = "Validation Error: Maximum score must be greater than zero.";
        return false;
    }
    // TC-06: 0 <= score <= maxScore
    if (score < 0.0) {
        errorMessage = "Validation Error: Score cannot be negative.";
        return false;
    }
    if (score > maxScore) {
        std::ostringstream oss;
        oss << "Validation Error: Score (" << score << ") exceeds maximum allowed score (" << maxScore << ").";
        errorMessage = oss.str();
        return false;
    }
    return true;
}

double Mark::calculatePercentage() const {
    if (maxScore <= 0.0) return 0.0;
    return (score / maxScore) * 100.0;
}

std::string Mark::serialize() const {
    return markId + "|" + studentId + "|" + subject + "|" + assessment + "|" +
           std::to_string(score) + "|" + std::to_string(maxScore);
}

Mark Mark::deserialize(const std::string& line) {
    std::vector<std::string> parts = Utils::split(line, '|');
    if (parts.size() >= 6) {
        double sc = 0.0;
        double mx = 100.0;
        try {
            sc = std::stod(parts[4]);
            mx = std::stod(parts[5]);
        } catch (...) {}
        return Mark(parts[0], parts[1], parts[2], parts[3], sc, mx);
    }
    return Mark();
}
