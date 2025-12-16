#include "Instructor.h"
#include "Utils.h"
#include <iostream>
using namespace std;


Instructor::Instructor(string i, string n, string p)
: User(i, n, p, "Instructor") {}


void Instructor::dashboard() {
    header("Instructor Dashboard");
    cout << "1. Create Course" << endl;
    cout << "2. Upload Assignment" << endl;
    cout << "3. View Submissions" << endl;
    cout << "4. Grade Assignment" << endl;
    cout << "5. Logout" << endl;
}