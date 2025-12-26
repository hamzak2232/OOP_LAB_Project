#include "Admin.h"
#include "LMS.h"
#include "Student.h"
#include "Faculty.h"
#include "Course.h"
#include "UIUtils.h"
#include <iostream>
#include <fstream>

void Admin::showMenu(LMS* lms) {
    while (true) {
        UIUtils::printHeader("ADMIN DASHBOARD - " + name);
        std::cout << "1. Create New Course\n";
        std::cout << "2. Register New Student\n";
        std::cout << "3. Register New Faculty\n";
        std::cout << "4. View All Users\n";
        std::cout << "5. View All Courses\n";
        std::cout << "6. Generate Seating Plan\n";
        std::cout << "7. Generate Fee Challan\n";
        std::cout << "8. Organized Reports (View/Save)\n";
        std::cout << "0. Logout\n";
        UIUtils::printFooter();

        int choice = UIUtils::getIntInput("Enter Choice: ");

        if (choice == 0) return; // Returns to LMS main loop, which will handle logout

        if (choice == 1) {
            UIUtils::printHeader("CREATE COURSE");
            std::string code = UIUtils::getStringInput("Enter Course Code: ");
            std::string title = UIUtils::getStringInput("Enter Name: ");
            std::string instId = UIUtils::getStringInput("Enter Faculty ID: ");
            
            if (lms->registerCourse(code, title, instId)) {
                std::cout << "Course Created and Assigned to " << instId << " Successfully.\n";
            } else {
                std::cout << "Error: Could not create course. Check if ID exists and is Faculty, or if code is unique.\n";
            }
            UIUtils::pause();
        }
        else if (choice == 2) {
            UIUtils::printHeader("REGISTER STUDENT");
            std::string id = UIUtils::getStringInput("Enter Student Reg Number: ");
            if (lms->findUser(id)) {
                std::cout << "Error: User ID already exists!\n";
            } else {
                std::string name = UIUtils::getStringInput("Enter Name: ");
                std::string pass = UIUtils::getStringInput("Enter Password: ");
                std::string dept = UIUtils::getStringInput("Enter Department: ");
                Student* s = new Student(id, name, pass);
                s->setDepartment(dept);
                lms->users.push_back(s);
                std::cout << "Student Registered.\n";
            }
            UIUtils::pause();
        }
        else if (choice == 3) {
            UIUtils::printHeader("REGISTER FACULTY");
            std::string id = UIUtils::getStringInput("Enter Employee ID: ");
            if (lms->findUser(id)) {
                std::cout << "Error: User ID already exists!\n";
            } else {
                std::string name = UIUtils::getStringInput("Enter Name: ");
                std::string pass = UIUtils::getStringInput("Enter Password: ");
                std::string dept = UIUtils::getStringInput("Enter Department: ");
                Faculty* f = new Faculty(id, name, pass);
                f->setDepartment(dept);
                lms->users.push_back(f);
                std::cout << "Faculty Registered.\n";
            }
            UIUtils::pause();
        }
        else if (choice == 4) {
            UIUtils::printHeader("ALL USERS");
            for (auto* u : lms->users) {
                std::cout << "[" << u->getUserType() << "] " << u->getName() << " (" << u->getID() << ")\n";
            }
            UIUtils::pause();
        }
        else if (choice == 5) {
            UIUtils::printHeader("ALL COURSES");
            for (auto* c : lms->courses) {
                c->viewCourseDetails();
                std::cout << "----------------\n";
            }
            UIUtils::pause();
        }
        else if (choice == 6) {
            UIUtils::printHeader("GENERATE SEATING PLAN");
            std::string code = UIUtils::getStringInput("Course Code: ");
            std::string eid = UIUtils::getStringInput("Exam ID: ");
            std::string date = UIUtils::getStringInput("Date (YYYY-MM-DD): ");
            std::string room = UIUtils::getStringInput("Room: ");
            lms->generateSeatingPlan(eid, code, date, room);
            std::cout << "Seating Plan Generated.\n";
            UIUtils::pause();
        }
        else if (choice == 7) {
            UIUtils::printHeader("GENERATE FEE CHALLAN");
            std::string sid = UIUtils::getStringInput("Student ID: ");
            std::string sem = UIUtils::getStringInput("Semester: ");
            float tui = UIUtils::getFloatInput("Tuition Fee: ");
            float lab = UIUtils::getFloatInput("Lab Fee: ");
            float lib = UIUtils::getFloatInput("Library Fee: ");
            std::string due = UIUtils::getStringInput("Due Date (YYYY-MM-DD): ");
            lms->generateFeeChallan(sid, sem, tui, lab, lib, due);
            std::cout << "Fee Challan Generated.\n";
            UIUtils::pause();
        }
        else if (choice == 8) {
            UIUtils::printHeader("ORGANIZED USER REPORT");
            std::string report = lms->getOrganizedUserReport();
            std::cout << report << "\n";
            
            std::string save = UIUtils::getStringInput("Save to file 'report.txt'? (y/n): ");
            if (save == "y" || save == "Y") {
                std::ofstream file("report.txt");
                if (file.is_open()) {
                    file << report;
                    file.close();
                    std::cout << "Report saved to report.txt\n";
                } else {
                    std::cout << "Error saving file.\n";
                }
            }
            UIUtils::pause();
        }
    }
}
