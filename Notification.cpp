#include "Notification.h"
#include <fstream>
#include <iostream>
using namespace std;

void Notification::send(string userID, string message) {
    ofstream out("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/notifications.txt", ios::app);
    out << userID << "|" << message << endl;
    out.close();
}

void Notification::view(string userID) {
    ifstream in("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/notifications.txt");
    string line;

    while (getline(in, line))
        if (line.find(userID + "|") == 0)
            cout << line << endl;

    in.close();
}
