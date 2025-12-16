#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>
using namespace std;

class Notification {
public:
    static void send(string userID, string message);
    static void view(string userID);
};

#endif
