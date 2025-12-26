#include "SeatingPlan.h"

SeatingPlan::SeatingPlan(std::string eid, std::string code, std::string d, std::string r)
    : examId(eid), courseCode(code), date(d), room(r) {}

void SeatingPlan::assignSeats(std::vector<std::string> studentIds) {
    int seatNumber = 1;
    for (const auto& studentId : studentIds) {
        studentSeats[studentId] = seatNumber++;
    }
}

int SeatingPlan::getSeat(const std::string& studentId) const {
    auto it = studentSeats.find(studentId);
    if (it != studentSeats.end()) {
        return it->second;
    }
    return -1;  // Not found
}
