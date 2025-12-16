#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <string>
using namespace std;

class Attendance {
public:
    static void mark(string studentID, string courseCode, bool present);
    static void view(string studentID);
};

#endif
