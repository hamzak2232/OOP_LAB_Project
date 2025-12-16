#include "Discussion.h"
#include <fstream>
#include <iostream>
using namespace std;

void Discussion::postMessage(string courseCode, string userID, string message) {
    ofstream out("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/discussions.txt", ios::app);
    out << courseCode << "|" << userID << "|" << message << endl;
    out.close();
}

void Discussion::viewMessages(string courseCode) {
    ifstream in("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/discussions.txt");
    string line;

    while (getline(in, line)) {
        if (line.find(courseCode + "|") == 0)
            cout << line << endl;
    }
    in.close();
}
