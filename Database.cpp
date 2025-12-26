#include "Database.h"
#include <iostream>
#include <vector>

void Database::saveUsers(const std::vector<User*>& users) {
    std::ofstream file("users.txt");
    if (!file.is_open()) return;

    for (auto u : users) {
        file << u->getUserType() << "|" << u->getID() << "|" << u->getName() << "|" << u->getPassword() << "|"
             << u->getMobileNo() << "|" << u->getPhoneNo() << "|" << u->getPersonalEmail() << "|"
             << u->getUniversityEmail() << "|" << u->getCurrentAddress() << "|" << u->getPermanentAddress() << "|" << u->getDepartment();
        
        if (u->getUserType() == "Student") {
            Student* s = static_cast<Student*>(u);
            file << "|" << s->getEnrollmentNo() << "|" << s->getRegistrationNo() << "|" << s->getFatherName() << "|"
                 << s->getProgram() << "|" << s->getIntakeSemester() << "|" << s->getMaxSemester() << "|" << s->getDegreeDuration();
        } else if (u->getUserType() == "Faculty") {
             Faculty* f = static_cast<Faculty*>(u);
            file << "|" << f->getEmployeeId() << "|" << f->getDesignation() << "|" << f->getOfficeHours() << "|";
            for (size_t i = 0; i < f->getTeachableCourses().size(); ++i) {
                file << f->getTeachableCourses()[i] << (i == f->getTeachableCourses().size() - 1 ? "" : ";");
            }
        }
        file << "\n";
    }
}

std::vector<User*> Database::loadUsers() {
    std::vector<User*> users;
    std::ifstream file("users.txt");
    if (!file.is_open()) return users;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string type, id, name, pass, mobile, phone, pEmail, uEmail, cAddr, pAddr, dept;
        std::getline(ss, type, '|');
        std::getline(ss, id, '|');
        std::getline(ss, name, '|');
        std::getline(ss, pass, '|');
        std::getline(ss, mobile, '|');
        std::getline(ss, phone, '|');
        std::getline(ss, pEmail, '|');
        std::getline(ss, uEmail, '|');
        std::getline(ss, cAddr, '|');
        std::getline(ss, pAddr, '|');
        std::getline(ss, dept, '|');

        User* u = nullptr;
        if (type == "Admin") {
            u = new Admin(id, name, pass);
        } else if (type == "Student") {
            u = new Student(id, name, pass);
            Student* s = static_cast<Student*>(u);
            std::string enroll, reg, father, prog, intake, maxSem, dur;
            std::getline(ss, enroll, '|');
            std::getline(ss, reg, '|');
            std::getline(ss, father, '|');
            std::getline(ss, prog, '|');
            std::getline(ss, intake, '|');
            std::getline(ss, maxSem, '|');
            std::getline(ss, dur, '|');
            s->setEnrollmentNo(enroll);
            s->setRegistrationNo(reg);
            s->setFatherName(father);
            s->setProgram(prog);
            s->setIntakeSemester(intake);
            s->setMaxSemester(maxSem);
            s->setDegreeDuration(dur);
        } else if (type == "Faculty") {
            u = new Faculty(id, name, pass);
            Faculty* f = static_cast<Faculty*>(u);
            std::string empId, desig, hours, teachableStr;
            std::getline(ss, empId, '|');
            std::getline(ss, desig, '|');
            std::getline(ss, hours, '|');
            std::getline(ss, teachableStr, '|');
            f->setEmployeeId(empId);
            f->setDesignation(desig);
            f->setOfficeHours(hours);
            
            std::vector<std::string> teachable;
            std::stringstream tss(teachableStr);
            std::string t;
            while (std::getline(tss, t, ';')) {
                if (!t.empty()) teachable.push_back(t);
            }
            f->setTeachableCourses(teachable);
        }

        if (u) {
            u->setMobileNo(mobile);
            u->setPhoneNo(phone);
            u->setPersonalEmail(pEmail);
            u->setUniversityEmail(uEmail);
            u->setCurrentAddress(cAddr);
            u->setPermanentAddress(pAddr);
            u->setDepartment(dept);
            users.push_back(u);
        }
    }
    return users;
}

void Database::saveCourses(const std::vector<Course*>& courses) {
    std::ofstream file("courses.txt");
    if (!file.is_open()) return;

    for (auto c : courses) {
        file << c->getCourseCode() << "|" << c->getCourseName() << "|" 
             << (c->getInstructor() ? c->getInstructor()->getID() : "NONE") << "|"
             << c->getCreditHours() << "|" << c->getMajor() << "|" << c->getOfferedCourseTitle() << "|"
             << c->getClassSection() << "|" << c->getSemester() << "\n";
    }
}

std::vector<Course*> Database::loadCourses() {
    std::vector<Course*> courses;
    std::ifstream file("courses.txt");
    if (!file.is_open()) return courses;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string code, name, instId, credits, major, title, section, sem;
        std::getline(ss, code, '|');
        std::getline(ss, name, '|');
        std::getline(ss, instId, '|');
        std::getline(ss, credits, '|');
        std::getline(ss, major, '|');
        std::getline(ss, title, '|');
        std::getline(ss, section, '|');
        std::getline(ss, sem, '|');

        Course* c = new Course(code, name);
        c->setCreditHours(credits.empty() ? 0 : std::stoi(credits));
        c->setMajor(major);
        c->setOfferedCourseTitle(title);
        c->setClassSection(section);
        c->setSemester(sem);
        // Note: Instructor will be linked later by main controller
        courses.push_back(c);
    }
    return courses;
}

void Database::saveAttendance(const std::vector<Attendance>& attendanceRecords) {
    std::ofstream file("attendance.txt");
    if (!file.is_open()) return;
    for (const auto& r : attendanceRecords) {
        file << r.getCourseCode() << "|" << r.getStudentId() << "|" << r.getDate() << "|" << r.isPresent() << "\n";
    }
}

std::vector<Attendance> Database::loadAttendance() {
    std::vector<Attendance> records;
    std::ifstream file("attendance.txt");
    if (!file.is_open()) return records;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string code, sid, date, present;
        std::getline(ss, code, '|');
        std::getline(ss, sid, '|');
        std::getline(ss, date, '|');
        std::getline(ss, present, '|');
        records.push_back(Attendance(code, sid, date, present == "1"));
    }
    return records;
}
void Database::saveGrades(const std::vector<Grade>& grades) {
    std::ofstream file("grades.txt");
    if (!file.is_open()) return;
    for (const auto& g : grades) {
        file << g.getStudentId() << "|" << g.getCourseCode() << "|" << g.getMidterm() << "|" 
             << g.getFinalExam() << "|" << g.getAssignments() << "|" << g.getQuizzes() << "|" 
             << g.getTotal() << "|" << g.getLetterGrade() << "|" << g.getGPA() << "\n";
    }
}

std::vector<Grade> Database::loadGrades() {
    std::vector<Grade> records;
    std::ifstream file("grades.txt");
    if (!file.is_open()) return records;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string sid, code, mid, fin, asg, quiz, tot, let, gpa;
        std::getline(ss, sid, '|');
        std::getline(ss, code, '|');
        std::getline(ss, mid, '|');
        std::getline(ss, fin, '|');
        std::getline(ss, asg, '|');
        std::getline(ss, quiz, '|');
        std::getline(ss, tot, '|');
        std::getline(ss, let, '|');
        std::getline(ss, gpa, '|');
        
        Grade g(sid, code);
        g.setMidterm(std::stof(mid));
        g.setFinalExam(std::stof(fin));
        g.setAssignments(std::stof(asg));
        g.setQuizzes(std::stof(quiz));
        g.calculateGrade(); // Recalculate just in case
        records.push_back(g);
    }
    return records;
}
void Database::saveSeatingPlans(const std::vector<SeatingPlan>& plans) {
    std::ofstream file("seating_plans.txt");
    if (!file.is_open()) return;
    for (const auto& p : plans) {
        file << p.getExamId() << "|" << p.getCourseCode() << "|" << p.getDate() << "|" << p.getRoom();
        for (const auto& pair : p.getStudentSeats()) {
            file << "|" << pair.first << ":" << pair.second;
        }
        file << "\n";
    }
}

std::vector<SeatingPlan> Database::loadSeatingPlans() {
    std::vector<SeatingPlan> records;
    std::ifstream file("seating_plans.txt");
    if (!file.is_open()) return records;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string eid, code, date, room;
        std::getline(ss, eid, '|');
        std::getline(ss, code, '|');
        std::getline(ss, date, '|');
        std::getline(ss, room, '|');
        
        SeatingPlan p(eid, code, date, room);
        std::string mapping;
        while (std::getline(ss, mapping, '|')) {
            size_t colon = mapping.find(':');
            if (colon != std::string::npos) {
                std::string sid = mapping.substr(0, colon);
                int seat = std::stoi(mapping.substr(colon + 1));
                p.studentSeats[sid] = seat;
            }
        }
        records.push_back(p);
    }
    return records;
}
void Database::saveFeeChallans(const std::vector<FeeChallan>& challans) {
    std::ofstream file("fee_challans.txt");
    if (!file.is_open()) return;
    for (const auto& f : challans) {
        file << f.getChallanId() << "|" << f.getStudentId() << "|" << f.getSemester() << "|" 
             << f.getTuitionFee() << "|" << f.getLabFee() << "|" << f.getLibraryFee() << "|" 
             << f.getDueDate() << "|" << f.isPaid() << "\n";
    }
}

std::vector<FeeChallan> Database::loadFeeChallans() {
    std::vector<FeeChallan> records;
    std::ifstream file("fee_challans.txt");
    if (!file.is_open()) return records;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string cid, sid, sem, tui, lab, lib, due, paid;
        std::getline(ss, cid, '|');
        std::getline(ss, sid, '|');
        std::getline(ss, sem, '|');
        std::getline(ss, tui, '|');
        std::getline(ss, lab, '|');
        std::getline(ss, lib, '|');
        std::getline(ss, due, '|');
        std::getline(ss, paid, '|');
        
        FeeChallan f(cid, sid, sem, std::stof(tui), std::stof(lab), std::stof(lib), due);
        f.setPaid(paid == "1");
        records.push_back(f);
    }
    return records;
}
