#ifndef USER_H
#define USER_H
#include <string>
using namespace std;


class User {
protected:
    string id, name, password, role;
public:
    User(string, string, string, string);
    virtual void dashboard() = 0;
    string getID();
    string getRole();
    bool authenticate(string);
    virtual ~User() {}
};
#endif