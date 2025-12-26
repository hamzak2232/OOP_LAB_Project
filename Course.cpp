#include "Course.h"
#include "Student.h"
#include "Faculty.h"

Course::Course(std::string code, std::string name)
    : courseCode(code), courseName(name), instructor(nullptr), 
      creditHours(3), major("N/A"), offeredCourseTitle(name), 
      classSection(""), semester("") {}

void Course::setInstructor(Faculty* faculty) {
    this->instructor = faculty;
}

void Course::addStudent(Student* student) {
    enrolledStudents.push_back(student);
}

void Course::addAssignment(Assignment* assignment) {
    assignments.push_back(assignment);
}

void Course::viewCourseDetails() const {
    std::cout << "Course: " << courseName << " (" << courseCode << ")\n";
    if (instructor) {
        std::cout << "Instructor: " << instructor->getName() << "\n";
    } else {
        std::cout << "Instructor: TBD\n";
    }
    std::cout << "Enrolled Students: " << enrolledStudents.size() << "\n";
    std::cout << "Assignments: " << assignments.size() << "\n";
}
