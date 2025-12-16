#include "Submission.h"
Submission::Submission(string s, string a)
: studentID(s), assignmentTitle(a), marks(0) {}
void Submission::grade(int m) { marks = m; }