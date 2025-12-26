#ifndef FEECHALLAN_H
#define FEECHALLAN_H

#include <string>

class FeeChallan {
private:
    std::string challanId;
    std::string studentId;
    std::string semester;
    float tuitionFee;
    float labFee;
    float libraryFee;
    float total;
    std::string dueDate;
    bool paid;

public:
    FeeChallan(std::string cid, std::string sid, std::string sem, 
               float tuition, float lab, float library, std::string due);
    
    // Calculate total
    void calculateTotal();
    
    // Getters
    std::string getChallanId() const { return challanId; }
    std::string getStudentId() const { return studentId; }
    std::string getSemester() const { return semester; }
    float getTuitionFee() const { return tuitionFee; }
    float getLabFee() const { return labFee; }
    float getLibraryFee() const { return libraryFee; }
    float getTotal() const { return total; }
    std::string getDueDate() const { return dueDate; }
    bool isPaid() const { return paid; }
    
    // Setters
    void setPaid(bool p) { paid = p; }
};

#endif
