#include "LMS.h"
#include "Admin.h"
#include "Student.h"
#include "Faculty.h"
#include "UIUtils.h"
#include <iostream>
#include <algorithm>

#include "Database.h"

LMS::LMS() : currentUser(nullptr) {
    loadState();
    if (users.empty()) {
        seedData();
        saveState();
    }
}

LMS::~LMS() {
    saveState();
    for (auto u : users) delete u;
    for (auto c : courses) delete c;
}

void LMS::loadState() {
    users = Database::loadUsers();
    courses = Database::loadCourses();
    attendanceRecords = Database::loadAttendance();
    grades = Database::loadGrades();
    seatingPlans = Database::loadSeatingPlans();
    feeChallans = Database::loadFeeChallans();
    
    // ...
}

void LMS::saveState() {
    Database::saveUsers(users);
    Database::saveCourses(courses);
    Database::saveAttendance(attendanceRecords);
    Database::saveGrades(grades);
    Database::saveSeatingPlans(seatingPlans);
    Database::saveFeeChallans(feeChallans);
}

void LMS::generateFeeChallan(std::string studentId, std::string semester, float tuition, float lab, float library, std::string dueDate) {
    static int challanId = 1000;
    std::string cid = "CHL-" + std::to_string(challanId++);
    feeChallans.push_back(FeeChallan(cid, studentId, semester, tuition, lab, library, dueDate));
}

std::vector<FeeChallan> LMS::getFeeChallans(std::string studentId) const {
    std::vector<FeeChallan> result;
    for (const auto& f : feeChallans) {
        if (f.getStudentId() == studentId) result.push_back(f);
    }
    return result;
}

void LMS::generateSeatingPlan(std::string examId, std::string courseCode, std::string date, std::string room) {
    Course* c = findCourse(courseCode);
    if (!c) return;
    
    SeatingPlan plan(examId, courseCode, date, room);
    std::vector<std::string> sids;
    for (auto* s : c->getEnrolledStudents()) {
        sids.push_back(s->getID());
    }
    plan.assignSeats(sids);
    seatingPlans.push_back(plan);
}

const SeatingPlan* LMS::getSeatingPlan(std::string courseCode) const {
    for (const auto& p : seatingPlans) {
        if (p.getCourseCode() == courseCode) return &p;
    }
    return nullptr;
}

void LMS::setGrade(std::string studentId, std::string courseCode, float mid, float final, float assign, float quiz) {
    for (auto& g : grades) {
        if (g.getStudentId() == studentId && g.getCourseCode() == courseCode) {
            g.setMidterm(mid);
            g.setFinalExam(final);
            g.setAssignments(assign);
            g.setQuizzes(quiz);
            g.calculateGrade();
            return;
        }
    }
    Grade newGrade(studentId, courseCode);
    newGrade.setMidterm(mid);
    newGrade.setFinalExam(final);
    newGrade.setAssignments(assign);
    newGrade.setQuizzes(quiz);
    newGrade.calculateGrade();
    grades.push_back(newGrade);
}

Grade* LMS::getGrade(std::string studentId, std::string courseCode) {
    for (auto& g : grades) {
        if (g.getStudentId() == studentId && g.getCourseCode() == courseCode) return &g;
    }
    return nullptr;
}

void LMS::markAttendance(std::string courseCode, std::string studentId, std::string date, bool present) {
    attendanceRecords.push_back(Attendance(courseCode, studentId, date, present));
}

std::vector<Attendance> LMS::getAttendance(std::string studentId, std::string courseCode) {
    std::vector<Attendance> result;
    for (const auto& record : attendanceRecords) {
        if (record.getStudentId() == studentId) {
            if (courseCode == "" || record.getCourseCode() == courseCode) {
                result.push_back(record);
            }
        }
    }
    return result;
}

User* LMS::findUser(std::string id) {
    for (auto u : users) {
        if (u->getID() == id) return u;
    }
    return nullptr;
}

void LMS::seedData() {
    // Create Admin
    users.push_back(new Admin("admin", "Admin User", "admin123"));

    // Create Faculty
    Faculty* f1 = new Faculty("emp1", "Dr. Ayesha", "pass");
    users.push_back(f1);

    // Create Student
    Student* s1 = new Student("01-1342", "Hamza", "pass");
    users.push_back(s1);

    // Create Course
    Course* c1 = new Course("CS212", "Calculus II");
    c1->setInstructor(f1); // Using public method
    c1->addStudent(s1);
    s1->enrollInCourse(c1); // Helper method handles bi-directional link effectively if used correctly
    courses.push_back(c1);
}

bool LMS::registerCourse(const std::string& code, const std::string& name, const std::string& instructorId) {
    if (findCourse(code)) return false; // Already exists
    
    User* user = findUser(instructorId);
    if (!user || user->getUserType() != "Faculty") return false; // Invalid instructor
    
    Faculty* instructor = static_cast<Faculty*>(user);
    Course* newCourse = new Course(code, name);
    newCourse->setInstructor(instructor);
    instructor->assignCourse(newCourse);
    courses.push_back(newCourse);
    saveState();
    return true;
}

bool LMS::deleteCourse(const std::string& code) {
    auto it = std::find_if(courses.begin(), courses.end(), [&](Course* c) {
        return c->getCourseCode() == code;
    });

    if (it == courses.end()) return false;

    Course* course = *it;

    // Remove from instructor
    if (course->getInstructor()) {
        course->getInstructor()->removeCourse(course);
    }

    // Remove from students
    for (auto* s : course->enrolledStudents) {
        s->unenrollFromCourse(course);
    }

    // Clean up associated data
    attendanceRecords.erase(std::remove_if(attendanceRecords.begin(), attendanceRecords.end(), [&](const Attendance& a) {
        return a.getCourseCode() == code;
    }), attendanceRecords.end());

    grades.erase(std::remove_if(grades.begin(), grades.end(), [&](const Grade& g) {
        return g.getCourseCode() == code;
    }), grades.end());
    
    seatingPlans.erase(std::remove_if(seatingPlans.begin(), seatingPlans.end(), [&](const SeatingPlan& p) {
        return p.getCourseCode() == code;
    }), seatingPlans.end());

    courses.erase(it);
    delete course;
    saveState();
    return true;
}

bool LMS::updateCourse(const std::string& code, const std::string& name, const std::string& instructorId) {
    Course* c = findCourse(code);
    if (!c) return false;

    if (!name.empty()) c->courseName = name;
    
    if (!instructorId.empty()) {
        User* user = findUser(instructorId);
        if (user && user->getUserType() == "Faculty") {
            Faculty* newInstructor = static_cast<Faculty*>(user);
            
            if (c->getInstructor() != newInstructor) {
                // Remove from old instructor
                if (c->getInstructor()) {
                    c->getInstructor()->removeCourse(c);
                }
                
                // Add to new instructor
                c->setInstructor(newInstructor);
                newInstructor->assignCourse(c);
            }
        }
    }
    
    saveState();
    return true;
}

std::string LMS::getOrganizedUserReport() const {
    std::stringstream ss;
    ss << "==========================================\n";
    ss << "BAHRIA UNIVERSITY LMS - ORGANIZED USER LIST\n";
    ss << "==========================================\n\n";

    // Grouping
    std::map<std::string, std::vector<Faculty*>> facultyByDept;
    std::map<std::string, std::vector<Student*>> studentsByDept;

    for (auto* u : users) {
        if (u->getUserType() == "Faculty") {
            facultyByDept[u->getDepartment()].push_back(static_cast<Faculty*>(u));
        } else if (u->getUserType() == "Student") {
            studentsByDept[u->getDepartment()].push_back(static_cast<Student*>(u));
        }
    }

    ss << "--- FACULTY MEMBERS BY DEPARTMENT ---\n";
    for (auto const& [dept, list] : facultyByDept) {
        ss << "\n[Department: " << (dept.empty() ? "Unassigned" : dept) << "]\n";
        for (auto* f : list) {
            ss << " - ID: " << f->getID() << " | Name: " << f->getName() << " | Designation: " << f->getDesignation() << "\n";
            ss << "   Teachable Courses: ";
            if (f->getTeachableCourses().empty()) ss << "None listed";
            else {
                for (size_t i = 0; i < f->getTeachableCourses().size(); ++i) {
                    ss << f->getTeachableCourses()[i] << (i == f->getTeachableCourses().size() - 1 ? "" : ", ");
                }
            }
            ss << "\n";
        }
    }

    ss << "\n\n--- STUDENTS BY DEPARTMENT ---\n";
    for (auto const& [dept, list] : studentsByDept) {
        ss << "\n[Department: " << (dept.empty() ? "Unassigned" : dept) << "]\n";
        for (auto* s : list) {
            ss << " - ID: " << s->getID() << " | Name: " << s->getName() << " | Program: " << s->getProgram() << "\n";
        }
    }

    return ss.str();
}

void LMS::run() {
    std::cout << "Welcome to Bahria University LMS Clone\n";
    while (true) {
        if (!currentUser) {
            login();
        } else {
            // Polymorphic call – passes 'this' (LMS pointer) so user can act on the system
            currentUser->showMenu(this); 
            // When showMenu returns, it means user logged out or wants to exit
            logout();
        }
    }
}

void LMS::login() {
    UIUtils::clearScreen();
    std::cout << "\n--- BAHIRA LMS LOGIN ---\n";
    
    std::string id, pass;
    std::cout << "ID: ";
    std::cin >> id;
    std::cout << "Password: ";
    std::cin >> pass;

    bool found = false;
    for (auto u : users) {
        if (u->getID() == id && u->authenticate(pass)) {
            currentUser = u;
            // UIUtils::printHeader("Login Successful!"); 
            // std::cout << "Welcome " << u->getName() << "\n";
            // UIUtils::pause();
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Invalid Credentials.\n";
        UIUtils::pause();
    }
}

void LMS::logout() {
    currentUser = nullptr;
    // std::cout << "Logged out.\n";
}

Course* LMS::findCourse(std::string code) {
    for(auto c : courses) {
        if(c->getCourseCode() == code) return c;
    }
    return nullptr;
}
