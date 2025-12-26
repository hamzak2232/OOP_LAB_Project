#include "Student.h"
#include "LMS.h" // Needed for context
#include "Course.h"
#include "UIUtils.h"
#include <iostream>
#include <algorithm>

void Student::enrollInCourse(Course* course) {
    if(!course) return;
    // Check if already enrolled
    for(auto c : enrolledCourses) {
        if(c == course) {
            std::cout << "Already enrolled in " << course->getCourseCode() << "\n";
            return;
        }
    }
    enrolledCourses.push_back(course);
    course->addStudent(this);
    std::cout << "Successfully enrolled in " << course->getCourseCode() << "\n";
}

void Student::unenrollFromCourse(Course* course) {
    if(!course) return;
    enrolledCourses.erase(std::remove(enrolledCourses.begin(), enrolledCourses.end(), course), enrolledCourses.end());
}

void Student::showMenu(LMS* lms) {
    while (true) {
        UIUtils::printHeader("STUDENT DASHBOARD - " + name);
        std::cout << "1. View Enrolled Courses and Assignments\n";
        std::cout << "2. Register for New Course\n";
        std::cout << "3. View Attendance\n";
        std::cout << "4. View Grades/Submissions\n";
        std::cout << "5. View Fee Challans\n";
        std::cout << "0. Logout\n";
        UIUtils::printFooter();

        int choice = UIUtils::getIntInput("Enter Choice: ");

        if (choice == 0) return;

        if (choice == 1) {
            UIUtils::printHeader("MY COURSES");
            if (enrolledCourses.empty()) {
                std::cout << "No courses enrolled.\n";
            } else {
                for (auto* c : enrolledCourses) {
                    std::cout << "* " << c->getCourseCode() << " : " << c->getCourseName() << "\n";
                    // Show assignments
                    std::cout << "  Assignments: \n";
                    for(auto* a : c->getAssignments()){
                        std::cout << "    - " << a->getTitle() << " (Marks: " << a->getMaxMarks() << ")\n";
                    }
                }
            }
            UIUtils::pause();
        }
        else if (choice == 2) {
            UIUtils::printHeader("REGISTER COURSE");
            std::string code = UIUtils::getStringInput("Enter Course Code to Register: ");
            Course* c = lms->findCourse(code);
            if (c) {
                enrollInCourse(c);
            } else {
                std::cout << "Course not found!\n";
            }
            UIUtils::pause();
        }
        else if (choice == 3) {
            UIUtils::printHeader("MY ATTENDANCE");
            std::string code = UIUtils::getStringInput("Enter Course Code (or press enter for all): ");
            auto records = lms->getAttendance(id, code);
            if (records.empty()) {
                std::cout << "No attendance records found.\n";
            } else {
                for (const auto& r : records) {
                    std::cout << "[" << r.getDate() << "] " << r.getCourseCode() << ": " 
                              << (r.isPresent() ? "Present" : "Absent") << "\n";
                }
            }
            UIUtils::pause();
        }
        else if (choice == 4) {
            UIUtils::printHeader("MY GRADES");
            std::string code = UIUtils::getStringInput("Enter Course Code: ");
            Grade* g = lms->getGrade(id, code);
            if (g) {
                std::cout << "Course: " << g->getCourseCode() << "\n";
                std::cout << "Midterm: " << g->getMidterm() << "\n";
                std::cout << "Final: " << g->getFinalExam() << "\n";
                std::cout << "Assignments: " << g->getAssignments() << "\n";
                std::cout << "Quizzes: " << g->getQuizzes() << "\n";
                std::cout << "---------------------\n";
                std::cout << "Total: " << g->getTotal() << "\n";
                std::cout << "Grade: " << g->getLetterGrade() << "\n";
                std::cout << "GPA: " << g->getGPA() << "\n";
            } else {
                std::cout << "No grade record found for this course.\n";
            }
            UIUtils::pause();
        }
        else if (choice == 5) {
            UIUtils::printHeader("MY FEE CHALLANS");
            auto records = lms->getFeeChallans(id);
            if (records.empty()) {
                std::cout << "No fee challans found.\n";
            } else {
                for (const auto& f : records) {
                    std::cout << "[" << f.getChallanId() << "] " << f.getSemester() << " - Total: " << f.getTotal() 
                              << " - Status: " << (f.isPaid() ? "Paid" : "Unpaid") << "\n";
                }
            }
            UIUtils::pause();
        }
    }
}
