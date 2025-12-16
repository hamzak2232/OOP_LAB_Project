#include "Admin.h"
#include "Utils.h"
#include <iostream>
using namespace std;


Admin::Admin(string i, string n, string p)
: User(i, n, p, "Admin") {}


void Admin::dashboard() {
    header("Admin Dashboard");
    cout << "1. Add User" << endl;
    cout << "2. View Users" << endl;
    cout << "3. View Courses" << endl;
    cout << "4. Logout" << endl;
}