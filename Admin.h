#ifndef ADMIN_H
#define ADMIN_H
#include "User.h"
class Admin : public User {
public:
    Admin(string, string, string);
    void dashboard() override;
};
#endif