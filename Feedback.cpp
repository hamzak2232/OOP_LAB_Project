#include "Feedback.h"
#include <fstream>
#include <iostream>
using namespace std;

void Feedback::add(string studentID, string courseCode, string comment) {
    ofstream out("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/feedback.txt", ios::app);
    out << studentID << " " << courseCode << " " << comment << endl;
    out.close();
}

void Feedback::view(string studentID) {
    ifstream in("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/feedback.txt");
    string sid, course, comment;

    while (in >> sid >> course) {
        getline(in, comment);
        if (sid == studentID)
            cout << course << " :" << comment << endl;
    }
    in.close();
}
