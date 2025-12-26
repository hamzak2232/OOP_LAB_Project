#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <string>

class Attendance {
private:
    std::string courseCode;
    std::string studentId;
    std::string date;  // YYYY-MM-DD format
    bool present;

public:
    Attendance(std::string code, std::string sid, std::string d, bool p);
    
    // Getters
    std::string getCourseCode() const { return courseCode; }
    std::string getStudentId() const { return studentId; }
    std::string getDate() const { return date; }
    bool isPresent() const { return present; }
    
    // Setters
    void setPresent(bool p) { present = p; }
};

#endif
