#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <string>
using namespace std;

class Feedback {
public:
    static void add(string studentID, string courseCode, string comment);
    static void view(string studentID);
};

#endif
