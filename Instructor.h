#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H
#include "User.h"
class Instructor : public User {
public:
    Instructor(string, string, string);
    void dashboard() override;
};
#endif