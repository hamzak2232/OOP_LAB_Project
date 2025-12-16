#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H
#include <string>
using namespace std;
class Assignment {
    string title, courseCode;
    int totalMarks;
public:
    Assignment(string, string, int);
};
#endif