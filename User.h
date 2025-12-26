#ifndef BAHRIA_LMS_USER_H
#define BAHRIA_LMS_USER_H

#include <string>
#include <iostream>

// Forward declaration
class LMS;

// Abstract Base Class
class User {
protected:
    std::string id;
    std::string name;
    std::string password;
    std::string userType; // "Admin", "Faculty", "Student"
    
    // Contact Information (common to all users)
    std::string mobileNo;
    std::string phoneNo;
    std::string personalEmail;
    std::string universityEmail;
    std::string currentAddress;
    std::string permanentAddress;
    std::string department;

public:
    User(std::string id, std::string name, std::string password, std::string type)
        : id(id), name(name), password(password), userType(type) {}

    virtual ~User() = default;

    // Getters
    std::string getID() const { return id; }
    std::string getName() const { return name; }
    std::string getPassword() const { return password; }
    std::string getUserType() const { return userType; }
    std::string getMobileNo() const { return mobileNo; }
    std::string getPhoneNo() const { return phoneNo; }
    std::string getPersonalEmail() const { return personalEmail; }
    std::string getUniversityEmail() const { return universityEmail; }
    std::string getCurrentAddress() const { return currentAddress; }
    std::string getPermanentAddress() const { return permanentAddress; }
    
    // Setters
    void setMobileNo(const std::string& mobile) { mobileNo = mobile; }
    void setPhoneNo(const std::string& phone) { phoneNo = phone; }
    void setPersonalEmail(const std::string& email) { personalEmail = email; }
    void setUniversityEmail(const std::string& email) { universityEmail = email; }
    void setCurrentAddress(const std::string& address) { currentAddress = address; }
    void setPermanentAddress(const std::string& address) { permanentAddress = address; }
    void setDepartment(const std::string& dept) { department = dept; }
    std::string getDepartment() const { return department; }

    // Authentication
    bool authenticate(const std::string& inputPassword) const {
        return password == inputPassword;
    }

    // Pure Virtual Function (making this an abstract class)
    virtual void showMenu(LMS* lms) = 0; // Context passed for interaction
};

#endif //BAHRIA_LMS_USER_H
