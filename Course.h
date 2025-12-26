#ifndef BAHRIA_LMS_COURSE_H
#define BAHRIA_LMS_COURSE_H

#include <string>
#include <vector>
#include <iostream>
#include "Assignment.h"

// Forward declarations
class Student;
class Faculty;

class Course {
private:
    friend class Admin;
    friend class Faculty;
    friend class Student;
    friend class WebServer;
    friend class LMS;

    std::string courseCode;
    std::string courseName;
    Faculty* instructor;
    std::vector<Student*> enrolledStudents;
    std::vector<Assignment*> assignments;
    
    // Extended Course Details
    int creditHours;
    std::string major;              // e.g., "N/A" or "Computer Science"
    std::string offeredCourseTitle; // May differ from courseName
    std::string classSection;       // e.g., "BS (CS)-5 (A) Morning"
    std::string semester;           // e.g., "Fall-2025"

public:
    Course(std::string code, std::string name);

    // Getters
    std::string getCourseCode() const { return courseCode; }
    std::string getCourseName() const { return courseName; }
    Faculty* getInstructor() const { return instructor; }
    const std::vector<Student*>& getEnrolledStudents() const { return enrolledStudents; }
    const std::vector<Assignment*>& getAssignments() const { return assignments; }
    int getCreditHours() const { return creditHours; }
    std::string getMajor() const { return major; }
    std::string getOfferedCourseTitle() const { return offeredCourseTitle; }
    std::string getClassSection() const { return classSection; }
    std::string getSemester() const { return semester; }
    
    // Setters
    void setCreditHours(int credits) { creditHours = credits; }
    void setMajor(const std::string& maj) { major = maj; }
    void setOfferedCourseTitle(const std::string& title) { offeredCourseTitle = title; }
    void setClassSection(const std::string& section) { classSection = section; }
    void setSemester(const std::string& sem) { semester = sem; }

    // Operations
    void setInstructor(Faculty* faculty);
    void addStudent(Student* student);
    void addAssignment(Assignment* assignment);
    void viewCourseDetails() const;
};

#endif //BAHRIA_LMS_COURSE_H
