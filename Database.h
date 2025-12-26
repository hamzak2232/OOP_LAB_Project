#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "User.h"
#include "Student.h"
#include "Faculty.h"
#include "Admin.h"
#include "Course.h"
#include "Attendance.h"
#include "Grade.h"
#include "SeatingPlan.h"
#include "FeeChallan.h"

class Database {
public:
    static void saveUsers(const std::vector<User*>& users);
    static std::vector<User*> loadUsers();

    static void saveCourses(const std::vector<Course*>& courses);
    static std::vector<Course*> loadCourses();

    static void saveAttendance(const std::vector<Attendance>& attendanceRecords);
    static std::vector<Attendance> loadAttendance();

    static void saveGrades(const std::vector<Grade>& grades);
    static std::vector<Grade> loadGrades();

    static void saveSeatingPlans(const std::vector<SeatingPlan>& plans);
    static std::vector<SeatingPlan> loadSeatingPlans();

    static void saveFeeChallans(const std::vector<FeeChallan>& challans);
    static std::vector<FeeChallan> loadFeeChallans();
};

#endif
