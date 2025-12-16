#ifndef DISCUSSION_H
#define DISCUSSION_H

#include <string>
using namespace std;

class Discussion {
public:
    static void postMessage(string courseCode, string userID, string message);
    static void viewMessages(string courseCode);
};

#endif
