#ifndef STUDENT_H
#define STUDENT_H
#include "User.h"
class Student : public User {
public:
    Student(string, string, string);
    void dashboard() override;
};
#endif