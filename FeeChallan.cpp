#include "FeeChallan.h"

FeeChallan::FeeChallan(std::string cid, std::string sid, std::string sem,
                       float tuition, float lab, float library, std::string due)
    : challanId(cid), studentId(sid), semester(sem), tuitionFee(tuition),
      labFee(lab), libraryFee(library), dueDate(due), paid(false) {
    calculateTotal();
}

void FeeChallan::calculateTotal() {
    total = tuitionFee + labFee + libraryFee;
}
