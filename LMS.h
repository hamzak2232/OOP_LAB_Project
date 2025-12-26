#ifndef BAHRIA_LMS_LMS_H
#define BAHRIA_LMS_LMS_H

#include <vector>
#include <map>
#include <string>
#include "User.h"
#include "Course.h"
#include "Attendance.h"
#include "Grade.h"
#include "SeatingPlan.h"
#include "FeeChallan.h"

// Forward Declaration
class WebServer;

class LMS {
private:
    friend class Admin;
    friend class Faculty;
    friend class Student;
    friend class WebServer;

    std::vector<User*> users;
    std::vector<Course*> courses;
    std::vector<Attendance> attendanceRecords;
    std::vector<Grade> grades;
    std::vector<SeatingPlan> seatingPlans;
    std::vector<FeeChallan> feeChallans;
    User* currentUser;

    void seedData(); // method to populate initial dummy data

public:
    LMS();
    ~LMS(); // Cleanup memory

    void run();
    void login();
    void logout();
    void registerUser(); // For Admin
    bool registerCourse(const std::string& code, const std::string& name, const std::string& instructorId);
    bool deleteCourse(const std::string& code);
    bool updateCourse(const std::string& code, const std::string& name, const std::string& instructorId);
    std::string getOrganizedUserReport() const;
    
    // Helper to find objects
    Course* findCourse(std::string code);
    User* findUser(std::string id);
    
    // Attendance
    void markAttendance(std::string courseCode, std::string studentId, std::string date, bool present);
    std::vector<Attendance> getAttendance(std::string studentId, std::string courseCode = "");
    
    // Grading
    void setGrade(std::string studentId, std::string courseCode, float mid, float final, float assign, float quiz);
    Grade* getGrade(std::string studentId, std::string courseCode);
    
    // Seating Plans
    void generateSeatingPlan(std::string examId, std::string courseCode, std::string date, std::string room);
    const SeatingPlan* getSeatingPlan(std::string courseCode) const;

    // Fee Challans
    void generateFeeChallan(std::string studentId, std::string semester, float tuition, float lab, float library, std::string dueDate);
    std::vector<FeeChallan> getFeeChallans(std::string studentId) const;

    void loadState();
    void saveState();
};

#endif //BAHRIA_LMS_LMS_H
