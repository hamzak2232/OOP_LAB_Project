#include "Grade.h"

Grade::Grade(std::string sid, std::string code)
    : studentId(sid), courseCode(code), midterm(0), finalExam(0), 
      assignments(0), quizzes(0), total(0), letterGrade("F"), gpa(0.0) {}

void Grade::calculateGrade() {
    // Weighted calculation: Midterm 30%, Final 40%, Assignments 20%, Quizzes 10%
    total = (midterm * 0.3f) + (finalExam * 0.4f) + (assignments * 0.2f) + (quizzes * 0.1f);
    
    // Bahria University Grading Scheme
    if (total >= 90) {
        letterGrade = "A+";
        gpa = 4.0f;
    } else if (total >= 85) {
        letterGrade = "A";
        gpa = 4.0f;
    } else if (total >= 80) {
        letterGrade = "A-";
        gpa = 3.67f;
    } else if (total >= 75) {
        letterGrade = "B+";
        gpa = 3.33f;
    } else if (total >= 71) {
        letterGrade = "B";
        gpa = 3.0f;
    } else if (total >= 68) {
        letterGrade = "B-";
        gpa = 2.67f;
    } else if (total >= 64) {
        letterGrade = "C+";
        gpa = 2.33f;
    } else if (total >= 61) {
        letterGrade = "C";
        gpa = 2.0f;
    } else if (total >= 58) {
        letterGrade = "C-";
        gpa = 1.67f;
    } else if (total >= 54) {
        letterGrade = "D+";
        gpa = 1.33f;
    } else if (total >= 50) {
        letterGrade = "D";
        gpa = 1.0f;
    } else {
        letterGrade = "F";
        gpa = 0.0f;
    }
}
