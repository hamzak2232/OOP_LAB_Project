#ifndef BAHRIA_LMS_STUDENT_H
#define BAHRIA_LMS_STUDENT_H

#include "User.h"
#include <vector>

// Forward declaration to avoid circular dependency
class Course; 

class Student : public User {
private:
    std::vector<Course*> enrolledCourses;
    
    // Student-Specific Profile Information
    std::string enrollmentNo;      // e.g., "01-124251-059"
    std::string registrationNo;    // e.g., "83442"
    std::string fatherName;
    std::string program;           // e.g., "BS (CS)"
    std::string intakeSemester;    // e.g., "Spring-2024"
    std::string maxSemester;       // e.g., "Fall-2029"
    std::string degreeDuration;    // e.g., "4 Years"

public:
    Student(std::string id, std::string name, std::string password)
        : User(id, name, password, "Student") {}

    void showMenu(LMS* lms) override;

    void enrollInCourse(Course* course);
    void unenrollFromCourse(Course* course);
    const std::vector<Course*>& getEnrolledCourses() const { return enrolledCourses; }
    
    // Getters for student-specific fields
    std::string getEnrollmentNo() const { return enrollmentNo; }
    std::string getRegistrationNo() const { return registrationNo; }
    std::string getFatherName() const { return fatherName; }
    std::string getProgram() const { return program; }
    std::string getIntakeSemester() const { return intakeSemester; }
    std::string getMaxSemester() const { return maxSemester; }
    std::string getDegreeDuration() const { return degreeDuration; }
    
    // Setters for student-specific fields
    void setEnrollmentNo(const std::string& enrollment) { enrollmentNo = enrollment; }
    void setRegistrationNo(const std::string& registration) { registrationNo = registration; }
    void setFatherName(const std::string& father) { fatherName = father; }
    void setProgram(const std::string& prog) { program = prog; }
    void setIntakeSemester(const std::string& intake) { intakeSemester = intake; }
    void setMaxSemester(const std::string& maxSem) { maxSemester = maxSem; }
    void setDegreeDuration(const std::string& duration) { degreeDuration = duration; }
};

#endif //BAHRIA_LMS_STUDENT_H
