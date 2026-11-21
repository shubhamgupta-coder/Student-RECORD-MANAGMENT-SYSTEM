#ifndef MARK_H
#define MARK_H

#include <string>
#include <vector>
#include "Utils.h"

class Mark {
private:
    std::string markId;
    std::string studentId;
    std::string subject;
    std::string assessment; // e.g. "Mid-Term", "Final Exam", "Assignment"
    double score;
    double maxScore;

public:
    Mark();
    Mark(const std::string& mId, const std::string& sId, const std::string& sub,
         const std::string& assess, double sc, double maxSc);

    // Getters
    std::string getMarkId() const;
    std::string getStudentId() const;
    std::string getSubject() const;
    std::string getAssessment() const;
    double getScore() const;
    double getMaxScore() const;

    // Methods from specification
    bool validateScore(std::string& errorMessage) const;
    double calculatePercentage() const;

    // Serialization helper
    std::string serialize() const;
    static Mark deserialize(const std::string& line);
};

#endif // MARK_H
