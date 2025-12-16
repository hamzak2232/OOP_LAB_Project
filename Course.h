#ifndef COURSE_H
#define COURSE_H
#include <string>
using namespace std;
class Course {
    string code, title, instructorID;
public:
    Course(string, string, string);
    string getCode();
    string getTitle();
};
#endif