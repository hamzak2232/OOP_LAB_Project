#ifndef SUBMISSION_H
#define SUBMISSION_H
#include <string>
using namespace std;
class Submission {
    string studentID, assignmentTitle;
    int marks;
public:
    Submission(string, string);
    void grade(int);
};
#endif