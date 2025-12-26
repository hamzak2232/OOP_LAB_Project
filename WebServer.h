#ifndef BAHRIA_LMS_WEBSERVER_H
#define BAHRIA_LMS_WEBSERVER_H

#include <string>
#include <map>
#include <vector>
#include "LMS.h" 

class WebServer {
private:
    int port;
    LMS* lms;
    
    // Helper to send data
    void sendResponse(int clientSocket, const std::string& status, const std::string& contentType, const std::string& body);
    void handleRequest(int clientSocket, const std::string& request);
    
    std::string readFile(const std::string& path);
    std::string getContentType(const std::string& path);
    
    // Parsers
    std::map<std::string, std::string> parseJson(const std::string& body);
    
    // API Handlers
    std::string handleLogin(const std::string& body);
    std::string getDashboardData(const std::string& userId);
    std::string getAllCourses(); // New: For registration list
    std::string getCourseDetails(const std::string& courseCode); // New: For details view
    std::string handleCreateCourse(const std::string& body); // Admin
    std::string handleUpdateCourse(const std::string& body); // Admin
    std::string handleDeleteCourse(const std::string& body); // Admin
    std::string handleAddAssignment(const std::string& body); // Faculty
    std::string handleEnroll(const std::string& body); // Student
    
    // User Management APIs
    std::string handleAddStudent(const std::string& body); // Admin: Add new student
    std::string handleAddFaculty(const std::string& body); // Admin: Add new faculty
    std::string getProfile(const std::string& userId); // Get full user profile
    std::string updateProfile(const std::string& body); // Update user profile
    
    // Attendance APIs
    std::string getAttendanceData(const std::string& userId, const std::string& courseCode);
    std::string handleMarkAttendance(const std::string& body);

    // Grading APIs
    std::string getGradeData(const std::string& userId, const std::string& courseCode);
    std::string handleSetGrade(const std::string& body);

    // Seating Plan APIs
    std::string getSeatingPlanData(const std::string& courseCode);
    std::string handleGenerateSeatingPlan(const std::string& body);

    // Fee APIs
    std::string getFeeChallanData(const std::string& userId);
    std::string handleGenerateChallan(const std::string& body);

public:
    WebServer(LMS* lmsSystem, int port = 8080);
    void start();
};

#endif //BAHRIA_LMS_WEBSERVER_H
