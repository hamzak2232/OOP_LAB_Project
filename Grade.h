#ifndef GRADE_H
#define GRADE_H

#include <string>

class Grade {
private:
    std::string studentId;
    std::string courseCode;
    float midterm;
    float finalExam;
    float assignments;
    float quizzes;
    float total;
    std::string letterGrade;
    float gpa;

public:
    Grade(std::string sid, std::string code);
    
    // Setters for marks
    void setMidterm(float m) { midterm = m; }
    void setFinalExam(float f) { finalExam = f; }
    void setAssignments(float a) { assignments = a; }
    void setQuizzes(float q) { quizzes = q; }
    
    // Calculate grade based on Bahria scheme
    void calculateGrade();
    
    // Getters
    std::string getStudentId() const { return studentId; }
    std::string getCourseCode() const { return courseCode; }
    float getMidterm() const { return midterm; }
    float getFinalExam() const { return finalExam; }
    float getAssignments() const { return assignments; }
    float getQuizzes() const { return quizzes; }
    float getTotal() const { return total; }
    std::string getLetterGrade() const { return letterGrade; }
    float getGPA() const { return gpa; }
};

#endif
