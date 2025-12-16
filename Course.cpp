#include "Course.h"
Course::Course(string c, string t, string i)
: code(c), title(t), instructorID(i) {}
string Course::getCode() { return code; }
string Course::getTitle() { return title; }