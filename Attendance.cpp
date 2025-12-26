#include "Attendance.h"

Attendance::Attendance(std::string code, std::string sid, std::string d, bool p)
    : courseCode(code), studentId(sid), date(d), present(p) {}
