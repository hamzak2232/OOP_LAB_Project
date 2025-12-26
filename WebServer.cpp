#include "WebServer.h"
#include "Student.h"
#include "Faculty.h"
#include "Admin.h"
#include "Course.h"
#include "Assignment.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

WebServer::WebServer(LMS* lmsSystem, int port) : lms(lmsSystem), port(port) {}

void WebServer::start() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        return;
    }

    listen(serverSocket, 10);
    std::cout << ">>> WEB SERVER STARTED on http://localhost:" << port << " <<<\n";

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) continue;

        char buffer[8192] = {0}; // Increased buffer for POST bodies
        int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytes > 0) {
            handleRequest(clientSocket, std::string(buffer, bytes));
        }

        // Graceful shutdown to prevent "Network Error" (Connection Reset)
        shutdown(clientSocket, SD_SEND);
        char dummy[1024];
        while(recv(clientSocket, dummy, sizeof(dummy), 0) > 0);
        closesocket(clientSocket);
    }
    
    WSACleanup();
}

void WebServer::handleRequest(int clientSocket, const std::string& request) {
    std::istringstream stream(request);
    std::string method, path;
    stream >> method >> path;

    if (path == "/") path = "/index.html";
    std::cout << "[" << method << "] " << path << "\n";

    // Extract Body if POST
    std::string body = "";
    if (method == "POST") {
        size_t bodyPos = request.find("\r\n\r\n");
        if (bodyPos != std::string::npos) {
            body = request.substr(bodyPos + 4);
        }
    }

    // --- API ROUTER ---
    std::string contentType = "application/json";
    std::string response = "";

    if (path == "/api/login" && method == "POST") {
        response = handleLogin(body);
    }
    else if (path.find("/api/dashboard") == 0 && method == "GET") { // Query param ?id=...
        // Extract ID from query
        size_t qPos = path.find("?id=");
        if (qPos != std::string::npos) {
            std::string uid = path.substr(qPos + 4);
            response = getDashboardData(uid);
        } else {
            response = "{\"error\":\"No ID provided\"}";
        }
    }
    else if (path == "/api/courses" && method == "GET") { // New: List all
        response = getAllCourses();
    }
    else if (path.find("/api/course/details") == 0 && method == "GET") { // New: Details ?code=...
        size_t qPos = path.find("?code=");
        if (qPos != std::string::npos) {
            std::string code = path.substr(qPos + 6);
            response = getCourseDetails(code);
        } else {
             response = "{\"error\":\"No Code provided\"}";
        }
    }
    else if (path == "/api/course" && method == "POST") {
        response = handleCreateCourse(body);
    }
    else if (path == "/api/course/update" && method == "POST") {
        response = handleUpdateCourse(body);
    }
    else if (path == "/api/course/delete" && method == "POST") {
        response = handleDeleteCourse(body);
    }
    else if (path == "/api/enroll" && method == "POST") {
        response = handleEnroll(body);
    }
    else if (path == "/api/assignment" && method == "POST") {
        response = handleAddAssignment(body);
    }
    // --- USER MANAGEMENT APIs ---
    else if (path == "/api/admin/add-student" && method == "POST") {
        response = handleAddStudent(body);
    }
    else if (path == "/api/admin/add-faculty" && method == "POST") {
        response = handleAddFaculty(body);
    }
    else if (path.find("/api/profile") == 0 && method == "GET") {
        size_t qPos = path.find("?id=");
        if (qPos != std::string::npos) {
            std::string uid = path.substr(qPos + 4);
            response = getProfile(uid);
        } else {
            response = "{\"error\":\"No ID provided\"}";
        }
    }
    else if (path == "/api/profile" && method == "PUT") {
        response = updateProfile(body);
    }
    else if (path.find("/api/attendance") == 0 && method == "GET") {
        size_t qPos = path.find("?studentId=");
        if (qPos != std::string::npos) {
            std::string sid = path.substr(qPos + 11);
            // Handle optional courseCode param
            std::string code = "";
            size_t cPos = path.find("&courseCode=");
            if (cPos != std::string::npos) {
                code = path.substr(cPos + 12);
                sid = sid.substr(0, sid.find("&"));
            }
            response = getAttendanceData(sid, code);
        } else {
            response = "{\"error\":\"No Student ID provided\"}";
        }
    }
    else if (path == "/api/attendance" && method == "POST") {
        response = handleMarkAttendance(body);
    }
    else if (path.find("/api/grades") == 0 && method == "GET") {
        size_t qPos = path.find("?studentId=");
        if (qPos != std::string::npos) {
            std::string sid = path.substr(qPos + 11);
            std::string code = "";
            size_t cPos = path.find("&courseCode=");
            if (cPos != std::string::npos) {
                code = path.substr(cPos + 12);
                sid = sid.substr(0, sid.find("&"));
            }
            response = getGradeData(sid, code);
        } else {
            response = "{\"error\":\"No Student ID provided\"}";
        }
    }
    else if (path == "/api/grades" && method == "POST") {
        response = handleSetGrade(body);
    }
    else if (path.find("/api/seating-plan") == 0 && method == "GET") {
        size_t qPos = path.find("?code=");
        if (qPos != std::string::npos) {
            std::string code = path.substr(qPos + 6);
            response = getSeatingPlanData(code);
        } else {
            response = "{\"error\":\"No Course Code provided\"}";
        }
    }
    else if (path == "/api/seating-plan" && method == "POST") {
        response = handleGenerateSeatingPlan(body);
    }
    else if (path.find("/api/fee-challan") == 0 && method == "GET") {
        size_t qPos = path.find("?studentId=");
        if (qPos != std::string::npos) {
            std::string sid = path.substr(qPos + 11);
            response = getFeeChallanData(sid);
        } else {
            response = "{\"error\":\"No Student ID provided\"}";
        }
    }
    else if (path == "/api/fee-challan" && method == "POST") {
        response = handleGenerateChallan(body);
    }
    else if (path == "/api/admin/report" && method == "GET") {
        contentType = "text/plain";
        response = lms->getOrganizedUserReport();
    }
    // --- STATIC FILES ---
    else if (method == "GET") {
        std::string filePath = "www" + path;
        response = readFile(filePath);
        if (!response.empty()) {
            contentType = getContentType(filePath);
        } else {
             sendResponse(clientSocket, "404 Not Found", "text/html", "<h1>404 Not Found</h1>");
             return;
        }
    } 
    else {
        sendResponse(clientSocket, "404 Not Found", "text/html", "API Not Found");
        return;
    }

    sendResponse(clientSocket, "200 OK", contentType, response);
}

void WebServer::sendResponse(int clientSocket, const std::string& status, const std::string& contentType, const std::string& body) {
    std::string response = "HTTP/1.1 " + status + "\r\n";
    response += "Access-Control-Allow-Origin: *\r\n"; // CORS
    response += "Content-Type: " + contentType + "\r\n";
    response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
    response += "Connection: close\r\n\r\n";
    response += body;
    send(clientSocket, response.c_str(), response.length(), 0);
}

std::string WebServer::readFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string WebServer::getContentType(const std::string& path) {
    if (path.ends_with(".html")) return "text/html";
    if (path.ends_with(".css")) return "text/css";
    if (path.ends_with(".js")) return "application/javascript";
    return "text/plain";
}

// --- HELPER PARSER ---
std::map<std::string, std::string> WebServer::parseJson(const std::string& body) {
    std::map<std::string, std::string> result;
    // VERY rough manual parser. Assumes keys are like "key":"value"
    // Does not handle nested objects or arrays well.
    // Simplified Manual JSON Parser
    size_t pos = 0;
    while(pos < body.length()) {
        size_t q1 = body.find('"', pos);
        if(q1 == std::string::npos) break;
        size_t q2 = body.find('"', q1+1);
        if(q2 == std::string::npos) break;
        std::string k = body.substr(q1+1, q2-q1-1);
        
        size_t colon = body.find(':', q2);
        if(colon == std::string::npos) break;
        
        // Find start of value: either a quote or a digit/char
        size_t valStart = colon + 1;
        while(valStart < body.length() && (body[valStart] == ' ' || body[valStart] == '\t' || body[valStart] == '\n' || body[valStart] == '\r')) valStart++;
        
        if (valStart < body.length() && body[valStart] == '"') {
            // String value
            size_t q3 = valStart;
            size_t q4 = body.find('"', q3+1);
            if (q4 == std::string::npos) break;
            result[k] = body.substr(q3+1, q4-q3-1);
            pos = q4 + 1;
        } else {
            // Unquoted value (e.g. number)
            size_t valEnd = body.find_first_of(",}", valStart);
            if (valEnd == std::string::npos) valEnd = body.length();
            std::string v = body.substr(valStart, valEnd - valStart);
            // Trim v
            v.erase(v.find_last_not_of(" \t\r\n") + 1);
            result[k] = v;
            pos = valEnd;
        }
    }
    return result;
}

// --- API IMPLEMENTATION ---

std::string WebServer::handleLogin(const std::string& body) {
    auto data = parseJson(body);
    std::string id = data["id"];
    std::string pass = data["password"];
    
    for (auto u : lms->users) {
        if (u->getID() == id && u->authenticate(pass)) {
            // Success
            std::string json = "{";
            json += "\"status\":\"success\",";
            json += "\"role\":\"" + u->getUserType() + "\",";
            json += "\"name\":\"" + u->getName() + "\",";
            json += "\"id\":\"" + u->getID() + "\"";
            json += "}";
            return json;
        }
    }
    return "{\"status\":\"error\", \"message\":\"Invalid Credentials\"}";
}

std::string WebServer::getDashboardData(const std::string& userId) {
    // Determine user role and fetch data
    User* user = nullptr;
    for(auto u : lms->users) { if(u->getID() == userId) user = u; }
    
    if(!user) return "{\"error\":\"User not found\"}";
    
    std::string json = "{ \"courses\": [";
    
    if (user->getUserType() == "Student") {
        Student* s = dynamic_cast<Student*>(user);
        const auto& list = s->getEnrolledCourses();
        for(size_t i=0; i<list.size(); ++i) {
            Course* c = list[i];
            json += "{\"code\":\"" + c->getCourseCode() + "\", \"name\":\"" + c->getCourseName() + "\", \"instructor\":\"" + (c->getInstructor() ? c->getInstructor()->getName() : "TBD") + "\"";
            json += ", \"creditHours\":" + std::to_string(c->getCreditHours());
            json += ", \"major\":\"" + c->getMajor() + "\"";
            json += ", \"offeredTitle\":\"" + c->getOfferedCourseTitle() + "\"";
            json += ", \"classSection\":\"" + c->getClassSection() + "\"";
            json += ", \"semester\":\"" + c->getSemester() + "\"";
            json += ", \"feeStatus\":\"Paid\"";  // Placeholder - will implement in Sprint 6
            json += "}";
            if(i < list.size()-1) json += ",";
        }
    } 
    else if (user->getUserType() == "Faculty") {
        // Faculty* f = dynamic_cast<Faculty*>(user);
        // We need a getter in Faculty or iterate
        // Faculty::assignedCourses is private... wait, WebServer is friend of LMS but not Faculty?
        // Ah, friend inheritance isn't transitive.
        // But we added friend WebServer to LMS.
        // We SHOULD add friend WebServer to Student, Admin, etc to access their private data dynamically.
        // OR rely on public getters.
        // Student has getEnrolledCourses (checking Student.h... yes)
        // Faculty... checking Faculty.h... NO public getter for assigned courses.
        // FIX: I will rely on iterating ALL courses and checking instructor.
        
        int count = 0;
        for(auto c : lms->courses) {
            if(c->getInstructor() && c->getInstructor()->getID() == userId) {
                if(count > 0) json += ",";
                json += "{\"code\":\"" + c->getCourseCode() + "\", \"name\":\"" + c->getCourseName() + "\"";
                json += ", \"creditHours\":" + std::to_string(c->getCreditHours());
                json += ", \"major\":\"" + c->getMajor() + "\"";
                json += ", \"offeredTitle\":\"" + c->getOfferedCourseTitle() + "\"";
                json += ", \"classSection\":\"" + c->getClassSection() + "\"";
                json += ", \"semester\":\"" + c->getSemester() + "\"";
                json += "}";
                count++;
            }
        }
    }
    else if (user->getUserType() == "Admin") {
        // Return ALL courses
        for(size_t i=0; i<lms->courses.size(); ++i) {
             Course* c = lms->courses[i];
             json += "{\"code\":\"" + c->getCourseCode() + "\", \"name\":\"" + c->getCourseName() + "\"";
             json += ", \"instructor\":\"" + (c->getInstructor() ? c->getInstructor()->getName() : "TBD") + "\"";
             json += ", \"creditHours\":" + std::to_string(c->getCreditHours());
             json += ", \"major\":\"" + c->getMajor() + "\"";
             json += ", \"offeredTitle\":\"" + c->getOfferedCourseTitle() + "\"";
             json += ", \"classSection\":\"" + c->getClassSection() + "\"";
             json += ", \"semester\":\"" + c->getSemester() + "\"";
             json += "}";
             if(i < lms->courses.size()-1) json += ",";
        }
    }
    
    json += "] }";
    return json;
}

std::string WebServer::handleCreateCourse(const std::string& body) {
    auto data = parseJson(body);
    std::string code = data["code"];
    std::string name = data["name"];
    std::string teacherId = data["teacherId"];
    
    if (code.empty()) return "{\"status\":\"error\", \"message\":\"Course code required\"}";
    
    if (lms->registerCourse(code, name, teacherId)) {
        // Since registerCourse only sets code and name, we update others manually
        Course* c = lms->findCourse(code);
        if (c) {
            if (!data["offeredTitle"].empty()) c->setOfferedCourseTitle(data["offeredTitle"]);
            if (!data["creditHours"].empty()) {
                try {
                    c->setCreditHours(std::stoi(data["creditHours"]));
                } catch (...) { c->setCreditHours(3); }
            }
            if (!data["major"].empty()) c->setMajor(data["major"]);
            if (!data["classSection"].empty()) c->setClassSection(data["classSection"]);
            if (!data["semester"].empty()) c->setSemester(data["semester"]);
        }
        return "{\"status\":\"success\", \"message\":\"Course created successfully\"}";
    }
    return "{\"status\":\"error\", \"message\":\"Failed to create course. Check if code is unique and Teacher ID is valid.\"}";
}

std::string WebServer::handleUpdateCourse(const std::string& body) {
    auto data = parseJson(body);
    std::string code = data["code"];
    std::string name = data["name"];
    std::string teacherId = data["teacherId"];

    if (lms->updateCourse(code, name, teacherId)) {
        Course* c = lms->findCourse(code);
        if (c) {
            if (!data["offeredTitle"].empty()) c->setOfferedCourseTitle(data["offeredTitle"]);
            if (!data["creditHours"].empty()) {
                try {
                    c->setCreditHours(std::stoi(data["creditHours"]));
                } catch (...) { c->setCreditHours(3); }
            }
            if (!data["major"].empty()) c->setMajor(data["major"]);
            if (!data["classSection"].empty()) c->setClassSection(data["classSection"]);
            if (!data["semester"].empty()) c->setSemester(data["semester"]);
        }
        return "{\"status\":\"success\", \"message\":\"Course updated successfully\"}";
    }
    return "{\"status\":\"error\", \"message\":\"Failed to update course.\"}";
}

std::string WebServer::handleDeleteCourse(const std::string& body) {
    auto data = parseJson(body);
    std::string code = data["code"];
    if (lms->deleteCourse(code)) {
        return "{\"status\":\"success\", \"message\":\"Course deleted successfully\"}";
    }
    return "{\"status\":\"error\", \"message\":\"Failed to delete course.\"}";
}

std::string WebServer::handleEnroll(const std::string& body) {
    auto data = parseJson(body);
    // {"studentId":"...", "courseCode":"..."}
    std::string sid = data["studentId"];
    std::string code = data["courseCode"];
    
    Student* s = nullptr;
    for(auto u : lms->users) if(u->getID() == sid) s = dynamic_cast<Student*>(u);
    
    Course* c = lms->findCourse(code);
    
    if (s && c) {
        s->enrollInCourse(c);
        return "{\"status\":\"success\"}";
    }
    return "{\"status\":\"error\"}";
}

std::string WebServer::handleAddAssignment(const std::string& body) {
    auto data = parseJson(body);
    // {"courseCode":"...", "title":"...", "marks":"10", "date":"..."}
    std::string code = data["courseCode"];
    Course* c = lms->findCourse(code);
    if(c) {
        static int id=500;
        int marks = std::stoi(data["marks"].empty() ? "0" : data["marks"]);
        Assignment* a = new Assignment(id++, data["title"], "Web Created", marks, data["date"]);
        c->addAssignment(a);
        return "{\"status\":\"success\"}";
    }
    return "{\"status\":\"error\"}";
}

// --- NEW HANDLERS ---

std::string WebServer::getAllCourses() {
    std::string json = "{ \"courses\": [";
    for(size_t i=0; i<lms->courses.size(); ++i) {
        Course* c = lms->courses[i];
        json += "{\"code\":\"" + c->getCourseCode() + "\", \"name\":\"" + c->getCourseName() + "\", \"instructor\":\"" + (c->getInstructor() ? c->getInstructor()->getName() : "TBD") + "\"";
        json += ", \"creditHours\":" + std::to_string(c->getCreditHours());
        json += ", \"major\":\"" + c->getMajor() + "\"";
        json += ", \"offeredTitle\":\"" + c->getOfferedCourseTitle() + "\"";
        json += ", \"classSection\":\"" + c->getClassSection() + "\"";
        json += ", \"semester\":\"" + c->getSemester() + "\"";
        json += "}";
        if(i < lms->courses.size()-1) json += ",";
    }
    json += "] }";
    return json;
}

std::string WebServer::getCourseDetails(const std::string& courseCode) {
    Course* target = lms->findCourse(courseCode);
    if (!target) return "{\"error\":\"Course not found\"}";

    // Basic Info
    std::string json = "{";
    json += "\"code\":\"" + target->getCourseCode() + "\",";
    json += "\"name\":\"" + target->getCourseName() + "\",";
    json += "\"instructor\":\"" + (target->getInstructor() ? target->getInstructor()->getName() : "TBD") + "\",";
    
    // Assignments
    json += "\"assignments\":[";
    const auto& asgs = target->getAssignments();
    for(size_t i=0; i<asgs.size(); ++i) {
        Assignment* a = asgs[i];
        json += "{\"title\":\"" + a->getTitle() + "\", \"marks\":\"" + std::to_string(a->getMaxMarks()) + "\", \"deadline\":\"" + a->getDeadline() + "\"}";
        if(i < asgs.size()-1) json += ",";
    }
    json += "]";
    json += "}";
    return json;
}

// --- USER MANAGEMENT HANDLERS ---

std::string WebServer::handleAddStudent(const std::string& body) {
    auto data = parseJson(body);
    
    // Required fields
    std::string id = data["id"];
    std::string name = data["name"];
    std::string password = data["password"];
    
    if (id.empty() || name.empty() || password.empty()) {
        return "{\"status\":\"error\", \"message\":\"Missing required fields\"}";
    }
    
    // Check if user already exists
    for (auto u : lms->users) {
        if (u->getID() == id) {
            return "{\"status\":\"error\", \"message\":\"User ID already exists\"}";
        }
    }
    
    // Create new student
    Student* s = new Student(id, name, password);
    
    // Set profile fields
    s->setEnrollmentNo(data["enrollmentNo"]);
    s->setRegistrationNo(data["registrationNo"]);
    s->setFatherName(data["fatherName"]);
    s->setProgram(data["program"]);
    s->setIntakeSemester(data["intakeSemester"]);
    s->setMaxSemester(data["maxSemester"]);
    s->setDegreeDuration(data["degreeDuration"]);
    s->setMobileNo(data["mobileNo"]);
    s->setPhoneNo(data["phoneNo"]);
    s->setPersonalEmail(data["personalEmail"]);
    s->setUniversityEmail(data["universityEmail"]);
    s->setCurrentAddress(data["currentAddress"]);
    s->setPermanentAddress(data["permanentAddress"]);
    
    lms->users.push_back(s);
    return "{\"status\":\"success\", \"message\":\"Student added successfully\"}";
}

std::string WebServer::handleAddFaculty(const std::string& body) {
    auto data = parseJson(body);
    
    // Required fields
    std::string id = data["id"];
    std::string name = data["name"];
    std::string password = data["password"];
    
    if (id.empty() || name.empty() || password.empty()) {
        return "{\"status\":\"error\", \"message\":\"Missing required fields\"}";
    }
    
    // Check if user already exists
    for (auto u : lms->users) {
        if (u->getID() == id) {
            return "{\"status\":\"error\", \"message\":\"User ID already exists\"}";
        }
    }
    
    // Create new faculty
    Faculty* f = new Faculty(id, name, password);
    
    // Set profile fields
    f->setEmployeeId(data["employeeId"]);
    f->setDepartment(data["department"]);
    f->setDesignation(data["designation"]);
    f->setOfficeHours(data["officeHours"]);
    
    if (!data["teachableCourses"].empty()) {
        std::vector<std::string> teachable;
        std::stringstream tss(data["teachableCourses"]);
        std::string t;
        while (std::getline(tss, t, ';')) {
            if (!t.empty()) teachable.push_back(t);
        }
        f->setTeachableCourses(teachable);
    }
    
    f->setMobileNo(data["mobileNo"]);
    f->setPhoneNo(data["phoneNo"]);
    f->setPersonalEmail(data["personalEmail"]);
    f->setUniversityEmail(data["universityEmail"]);
    f->setCurrentAddress(data["currentAddress"]);
    f->setPermanentAddress(data["permanentAddress"]);
    
    lms->users.push_back(f);
    return "{\"status\":\"success\", \"message\":\"Faculty added successfully\"}";
}

std::string WebServer::getProfile(const std::string& userId) {
    User* user = nullptr;
    for (auto u : lms->users) {
        if (u->getID() == userId) {
            user = u;
            break;
        }
    }
    
    if (!user) return "{\"error\":\"User not found\"}";
    
    std::string json = "{";
    json += "\"id\":\"" + user->getID() + "\",";
    json += "\"name\":\"" + user->getName() + "\",";
    json += "\"userType\":\"" + user->getUserType() + "\",";
    json += "\"mobileNo\":\"" + user->getMobileNo() + "\",";
    json += "\"phoneNo\":\"" + user->getPhoneNo() + "\",";
    json += "\"personalEmail\":\"" + user->getPersonalEmail() + "\",";
    json += "\"universityEmail\":\"" + user->getUniversityEmail() + "\",";
    json += "\"currentAddress\":\"" + user->getCurrentAddress() + "\",";
    json += "\"permanentAddress\":\"" + user->getPermanentAddress() + "\"";
    
    if (user->getUserType() == "Student") {
        Student* s = dynamic_cast<Student*>(user);
        json += ",\"enrollmentNo\":\"" + s->getEnrollmentNo() + "\"";
        json += ",\"registrationNo\":\"" + s->getRegistrationNo() + "\"";
        json += ",\"fatherName\":\"" + s->getFatherName() + "\"";
        json += ",\"program\":\"" + s->getProgram() + "\"";
        json += ",\"intakeSemester\":\"" + s->getIntakeSemester() + "\"";
        json += ",\"maxSemester\":\"" + s->getMaxSemester() + "\"";
        json += ",\"degreeDuration\":\"" + s->getDegreeDuration() + "\"";
    } else if (user->getUserType() == "Faculty") {
        Faculty* f = dynamic_cast<Faculty*>(user);
        json += ",\"employeeId\":\"" + f->getEmployeeId() + "\"";
        json += ",\"department\":\"" + f->getDepartment() + "\"";
        json += ",\"designation\":\"" + f->getDesignation() + "\"";
        json += ",\"officeHours\":\"" + f->getOfficeHours() + "\"";
    }
    
    json += "}";
    return json;
}

std::string WebServer::updateProfile(const std::string& body) {
    auto data = parseJson(body);
    std::string userId = data["id"];
    
    User* user = nullptr;
    for (auto u : lms->users) {
        if (u->getID() == userId) {
            user = u;
            break;
        }
    }
    
    if (!user) return "{\"status\":\"error\", \"message\":\"User not found\"}";
    
    // Update common fields
    if (!data["mobileNo"].empty()) user->setMobileNo(data["mobileNo"]);
    if (!data["phoneNo"].empty()) user->setPhoneNo(data["phoneNo"]);
    if (!data["personalEmail"].empty()) user->setPersonalEmail(data["personalEmail"]);
    if (!data["universityEmail"].empty()) user->setUniversityEmail(data["universityEmail"]);
    if (!data["currentAddress"].empty()) user->setCurrentAddress(data["currentAddress"]);
    if (!data["permanentAddress"].empty()) user->setPermanentAddress(data["permanentAddress"]);
    
    // Update type-specific fields
    if (user->getUserType() == "Student") {
        Student* s = dynamic_cast<Student*>(user);
        if (!data["enrollmentNo"].empty()) s->setEnrollmentNo(data["enrollmentNo"]);
        if (!data["registrationNo"].empty()) s->setRegistrationNo(data["registrationNo"]);
        if (!data["fatherName"].empty()) s->setFatherName(data["fatherName"]);
        if (!data["program"].empty()) s->setProgram(data["program"]);
        if (!data["intakeSemester"].empty()) s->setIntakeSemester(data["intakeSemester"]);
        if (!data["maxSemester"].empty()) s->setMaxSemester(data["maxSemester"]);
        if (!data["degreeDuration"].empty()) s->setDegreeDuration(data["degreeDuration"]);
    } else if (user->getUserType() == "Faculty") {
        Faculty* f = dynamic_cast<Faculty*>(user);
        if (!data["employeeId"].empty()) f->setEmployeeId(data["employeeId"]);
        if (!data["department"].empty()) f->setDepartment(data["department"]);
        if (!data["designation"].empty()) f->setDesignation(data["designation"]);
        if (!data["officeHours"].empty()) f->setOfficeHours(data["officeHours"]);
    }
    
    return "{\"status\":\"success\", \"message\":\"Profile updated successfully\"}";
}

std::string WebServer::getAttendanceData(const std::string& userId, const std::string& courseCode) {
    auto records = lms->getAttendance(userId, courseCode);
    std::string json = "{ \"attendance\": [";
    for (size_t i = 0; i < records.size(); ++i) {
        json += "{\"date\":\"" + records[i].getDate() + "\", \"course\":\"" + records[i].getCourseCode() + "\", \"status\":\"" + (records[i].isPresent() ? "Present" : "Absent") + "\"}";
        if (i < records.size() - 1) json += ",";
    }
    json += "] }";
    return json;
}

std::string WebServer::handleMarkAttendance(const std::string& body) {
    auto data = parseJson(body);
    std::string sid = data["studentId"];
    std::string code = data["courseCode"];
    std::string date = data["date"];
    bool present = (data["present"] == "true" || data["present"] == "y" || data["present"] == "1");

    if (sid.empty() || code.empty() || date.empty()) return "{\"status\":\"error\", \"message\":\"Missing fields\"}";

    lms->markAttendance(code, sid, date, present);
    return "{\"status\":\"success\"}";
}

std::string WebServer::getGradeData(const std::string& userId, const std::string& courseCode) {
    Grade* g = lms->getGrade(userId, courseCode);
    if (!g) return "{\"error\":\"Grade not found\"}";
    
    std::string json = "{";
    json += "\"midterm\":" + std::to_string(g->getMidterm()) + ",";
    json += "\"finalExam\":" + std::to_string(g->getFinalExam()) + ",";
    json += "\"assignments\":" + std::to_string(g->getAssignments()) + ",";
    json += "\"quizzes\":" + std::to_string(g->getQuizzes()) + ",";
    json += "\"total\":" + std::to_string(g->getTotal()) + ",";
    json += "\"letterGrade\":\"" + g->getLetterGrade() + "\",";
    json += "\"gpa\":" + std::to_string(g->getGPA());
    json += "}";
    return json;
}

std::string WebServer::handleSetGrade(const std::string& body) {
    auto data = parseJson(body);
    std::string sid = data["studentId"];
    std::string code = data["courseCode"];
    float mid = std::stof(data["midterm"].empty() ? "0" : data["midterm"]);
    float fin = std::stof(data["finalExam"].empty() ? "0" : data["finalExam"]);
    float asg = std::stof(data["assignments"].empty() ? "0" : data["assignments"]);
    float quiz = std::stof(data["quizzes"].empty() ? "0" : data["quizzes"]);

    if (sid.empty() || code.empty()) return "{\"status\":\"error\", \"message\":\"Missing fields\"}";

    lms->setGrade(sid, code, mid, fin, asg, quiz);
    return "{\"status\":\"success\"}";
}

std::string WebServer::getSeatingPlanData(const std::string& courseCode) {
    const SeatingPlan* p = lms->getSeatingPlan(courseCode);
    if (!p) return "{\"error\":\"Seating plan not found\"}";
    
    std::string json = "{";
    json += "\"examId\":\"" + p->getExamId() + "\",";
    json += "\"date\":\"" + p->getDate() + "\",";
    json += "\"room\":\"" + p->getRoom() + "\",";
    json += "\"seats\":[";
    const auto& seats = p->getStudentSeats();
    int i = 0;
    for (const auto& pair : seats) {
        json += "{\"studentId\":\"" + pair.first + "\", \"seat\":" + std::to_string(pair.second) + "}";
        if (++i < seats.size()) json += ",";
    }
    json += "]";
    json += "}";
    return json;
}

std::string WebServer::handleGenerateSeatingPlan(const std::string& body) {
    auto data = parseJson(body);
    std::string eid = data["examId"];
    std::string code = data["courseCode"];
    std::string date = data["date"];
    std::string room = data["room"];

    if (eid.empty() || code.empty() || date.empty() || room.empty()) return "{\"status\":\"error\", \"message\":\"Missing fields\"}";

    lms->generateSeatingPlan(eid, code, date, room);
    return "{\"status\":\"success\"}";
}

std::string WebServer::getFeeChallanData(const std::string& userId) {
    auto records = lms->getFeeChallans(userId);
    std::string json = "{ \"challans\": [";
    for (size_t i = 0; i < records.size(); ++i) {
        json += "{\"id\":\"" + records[i].getChallanId() + "\", \"semester\":\"" + records[i].getSemester() + "\", \"total\":" + std::to_string(records[i].getTotal()) + ", \"due\":\"" + records[i].getDueDate() + "\", \"status\":\"" + (records[i].isPaid() ? "Paid" : "Unpaid") + "\"}";
        if (i < records.size() - 1) json += ",";
    }
    json += "] }";
    return json;
}

std::string WebServer::handleGenerateChallan(const std::string& body) {
    auto data = parseJson(body);
    std::string sid = data["studentId"];
    std::string sem = data["semester"];
    float tui = std::stof(data["tuition"].empty() ? "0" : data["tuition"]);
    float lab = std::stof(data["lab"].empty() ? "0" : data["lab"]);
    float lib = std::stof(data["library"].empty() ? "0" : data["library"]);
    std::string due = data["dueDate"];

    if (sid.empty() || sem.empty() || due.empty()) return "{\"status\":\"error\", \"message\":\"Missing fields\"}";

    lms->generateFeeChallan(sid, sem, tui, lab, lib, due);
    return "{\"status\":\"success\"}";
}
