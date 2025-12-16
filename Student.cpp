#include "Student.h"
#include "Utils.h"
#include <iostream>
using namespace std;


Student::Student(string i, string n, string p)
: User(i, n, p, "Student") {}


void Student::dashboard() {
    header("Student Dashboard");
    cout << "1. View Courses" << endl;
    cout << "2. View Assignments" << endl;
    cout << "3. Submit Assignment" << endl;
    cout << "4. View Grades" << endl;
    cout << "5. Logout" << endl;
}