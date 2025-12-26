#ifndef BAHRIA_LMS_ADMIN_H
#define BAHRIA_LMS_ADMIN_H

#include "User.h"
#include <vector>

class Admin : public User {
public:
    Admin(std::string id, std::string name, std::string password)
        : User(id, name, password, "Admin") {}

    void showMenu(LMS* lms) override; // Implemented in .cpp

    // Additional Admin Methods will be implemented in .cpp or expanded here
};

#endif //BAHRIA_LMS_ADMIN_H
