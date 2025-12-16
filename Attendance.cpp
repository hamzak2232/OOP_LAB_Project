#include "Attendance.h"
#include <fstream>
#include <iostream>
using namespace std;

void Attendance::mark(string studentID, string courseCode, bool present) {
    ofstream out("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/attendance.txt", ios::app);
    out << studentID << " " << courseCode << " " << present << endl;
    out.close();
}

void Attendance::view(string studentID) {
    ifstream in("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/attendance.txt");
    string sid, course;
    bool status;

    while (in >> sid >> course >> status)
        if (sid == studentID)
            cout << course << " : " << (status ? "Present" : "Absent") << endl;

    in.close();
}
