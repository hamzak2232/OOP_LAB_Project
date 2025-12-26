#include "Faculty.h"
#include "LMS.h"
#include "Course.h"
#include "UIUtils.h"
#include <iostream>
#include <algorithm>

void Faculty::assignCourse(Course* course) {
    if(!course) return;
    assignedCourses.push_back(course);
    course->setInstructor(this);
}

void Faculty::removeCourse(Course* course) {
    if(!course) return;
    assignedCourses.erase(std::remove(assignedCourses.begin(), assignedCourses.end(), course), assignedCourses.end());
}

void Faculty::showMenu(LMS* lms) {
    while (true) {
        UIUtils::printHeader("FACULTY DASHBOARD - " + name);
        std::cout << "1. View Assigned Courses\n";
        std::cout << "2. Add Assignment to Course\n";
        std::cout << "3. Mark Attendance\n";
        std::cout << "4. Grade Students (Placeholder)\n";
        std::cout << "0. Logout\n";
        UIUtils::printFooter();

        int choice = UIUtils::getIntInput("Enter Choice: ");

        if (choice == 0) return;

        if (choice == 1) {
            UIUtils::printHeader("MY COURSES");
            for (auto* c : assignedCourses) {
                std::cout << "* " << c->getCourseCode() << " (" << c->getCourseName() << ")\n";
                std::cout << "  Students: " << "\n"; // Can add count logic
            }
            UIUtils::pause();
        }
        else if (choice == 2) {
            UIUtils::printHeader("ADD ASSIGNMENT");
            std::string code = UIUtils::getStringInput("Enter Course Code: ");
            
            // Validate ownership
            Course* target = nullptr;
            for(auto* c : assignedCourses){
                if(c->getCourseCode() == code) target = c;
            }

            if(target) {
                std::string title = UIUtils::getStringInput("Assignment Title: ");
                std::string desc = UIUtils::getStringInput("Description: ");
                int marks = UIUtils::getIntInput("Max Marks: ");
                std::string date = UIUtils::getStringInput("Deadline (YYYY-MM-DD): ");
                
                // ID generation simple
                static int assignId = 100;
                Assignment* newAsg = new Assignment(assignId++, title, desc, marks, date);
                target->addAssignment(newAsg);
                std::cout << "Assignment Added!\n";
            } else {
                std::cout << "You act not instructor for this course or it doesn't exist.\n";
            }
            UIUtils::pause();
        }
        else if (choice == 3) {
            UIUtils::printHeader("MARK ATTENDANCE");
            std::string code = UIUtils::getStringInput("Enter Course Code: ");
            Course* target = nullptr;
            for(auto* c : assignedCourses) if(c->getCourseCode() == code) target = c;

            if(target) {
                std::string date = UIUtils::getStringInput("Date (YYYY-MM-DD): ");
                // In a real app we'd list students, here we mark one for simplicity or loop
                std::string sid = UIUtils::getStringInput("Student ID: ");
                char status = UIUtils::getStringInput("Present (y/n): ")[0];
                lms->markAttendance(code, sid, date, (status == 'y' || status == 'Y'));
                std::cout << "Attendance Marked.\n";
            } else {
                std::cout << "Course not found or not assigned to you.\n";
            }
            UIUtils::pause();
        }
        else if (choice == 4) {
            UIUtils::printHeader("GRADE STUDENTS");
            std::string code = UIUtils::getStringInput("Enter Course Code: ");
            Course* target = nullptr;
            for(auto* c : assignedCourses) if(c->getCourseCode() == code) target = c;

            if(target) {
                std::string sid = UIUtils::getStringInput("Student ID: ");
                float mid = UIUtils::getFloatInput("Midterm Marks: ");
                float final = UIUtils::getFloatInput("Final Marks: ");
                float asg = UIUtils::getFloatInput("Assignments: ");
                float quiz = UIUtils::getFloatInput("Quizzes: ");
                
                lms->setGrade(sid, code, mid, final, asg, quiz);
                std::cout << "Grade Updated and Calculated.\n";
            } else {
                std::cout << "Course not found or not assigned to you.\n";
            }
            UIUtils::pause();
        }
    }
}
