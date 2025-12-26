#ifndef BAHRIA_LMS_FACULTY_H
#define BAHRIA_LMS_FACULTY_H

#include "User.h"
#include <vector>

// Forward declaration
class Course;

class Faculty : public User {
private:
    std::vector<Course*> assignedCourses;
    std::vector<std::string> teachableCourses; // Added this line
    
    // Faculty-Specific Profile Information
    std::string employeeId;
    std::string department;
    std::string designation;
    std::string officeHours;

public:
    Faculty(std::string id, std::string name, std::string password)
        : User(id, name, password, "Faculty") {}

    void showMenu(LMS* lms) override;

    void assignCourse(Course* course);
    void removeCourse(Course* course);
    const std::vector<Course*>& getAssignedCourses() const { return assignedCourses; }
    
    // Getters for faculty-specific fields
    std::string getEmployeeId() const { return employeeId; }
    std::string getDepartment() const { return department; }
    std::string getDesignation() const { return designation; }
    std::string getOfficeHours() const { return officeHours; }
    
    // Setters for faculty-specific fields
    void setEmployeeId(const std::string& empId) { employeeId = empId; }
    void setDepartment(const std::string& dept) { department = dept; }
    void setDesignation(const std::string& desig) { designation = desig; }
    void setOfficeHours(const std::string& hours) { officeHours = hours; }
    void setTeachableCourses(const std::vector<std::string>& courses) { teachableCourses = courses; }
    std::vector<std::string> getTeachableCourses() const { return teachableCourses; }
};

#endif //BAHRIA_LMS_FACULTY_H
