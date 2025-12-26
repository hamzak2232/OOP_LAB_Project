#ifndef SEATINGPLAN_H
#define SEATINGPLAN_H

#include <string>
#include <map>
#include <vector>

class SeatingPlan {
private:
    friend class Database;
    std::string examId;
    std::string courseCode;
    std::string date;
    std::string room;
    std::map<std::string, int> studentSeats;  // studentId -> seatNumber

public:
    SeatingPlan(std::string eid, std::string code, std::string d, std::string r);
    
    // Assign seats to students
    void assignSeats(std::vector<std::string> studentIds);
    
    // Get seat for specific student
    int getSeat(const std::string& studentId) const;
    
    // Getters
    std::string getExamId() const { return examId; }
    std::string getCourseCode() const { return courseCode; }
    std::string getDate() const { return date; }
    std::string getRoom() const { return room; }
    const std::map<std::string, int>& getStudentSeats() const { return studentSeats; }
};

#endif
