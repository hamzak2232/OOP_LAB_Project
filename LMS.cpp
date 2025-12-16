#include "LMS.h"
#include "Utils.h"
#include "Discussion.h"
#include "Attendance.h"
#include "Notification.h"
#include "Feedback.h"
#include "GradeBook.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <sstream>

using namespace std;

// ================= Validation =================
bool isValidStudentID(const string& id) {
    regex pattern(R"(\d{2}-\d{6}-\d{3})");
    return regex_match(id, pattern);
}

bool isCapitalizedName(const string& name) {
    if (name.empty()) return false;
    if (!isupper(name[0])) return false;
    for (int i = 1; i < name.length(); i++)
        if (!islower(name[i])) return false;
    return true;
}

bool isValidFullName(const string& fullName) {
    stringstream ss(fullName);
    string part;
    int count = 0;
    while (ss >> part) {
        if (!isCapitalizedName(part)) return false;
        count++;
    }
    return count >= 2;
}

// ================= LMS =================
void LMS::start() {
    filesystem::create_directory("data");

    int choice;
    while (true) {
        header("Bahria University LMS");
        cout << "1. Login\n2. Exit\nChoice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 2) break;
        if (choice != 1) continue;

        string id, pass;
        cout << "User ID: ";
        getline(cin, id);
        cout << "Password: ";
        getline(cin, pass);

        ifstream fin("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/users.txt");
        string uid, role, pwd, name;
        bool found = false;

        while (fin >> uid) {
            fin >> ws;
            getline(fin, name, ' ');
            fin >> role >> pwd;

            if (uid == id && pwd == pass) {
                found = true;

                // ================= ADMIN =================
                if (role == "Admin") {
                    int c;
                    do {
                        header("Admin Dashboard");
                        cout <<
                        "1. Add User\n"
                        "2. View Users\n"
                        "3. View Courses\n"
                        "4. Logout\nChoice: ";

                        cin >> c;
                        cin.ignore();

                        if (c == 1) {
                            string r, n, i, p;

                            while (true) {
                                cout << "Role (Admin/Instructor/Student): ";
                                getline(cin, r);
                                if (r == "Admin" || r == "Instructor" || r == "Student")
                                    break;
                            }

                            while (true) {
                                cout << "Full Name: ";
                                getline(cin, n);
                                if (isValidFullName(n)) break;
                            }

                            while (true) {
                                cout << "ID: ";
                                getline(cin, i);
                                if (r != "Student" || isValidStudentID(i)) break;
                            }

                            cout << "Password: ";
                            getline(cin, p);

                            ofstream out("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/users.txt", ios::app);
                            out << i << " " << n << " " << r << " " << p << endl;
                            out.close();

                            cout << "User added successfully.\n";
                            pause();
                        }

                        else if (c == 2) {
                            ifstream u("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/users.txt");
                            string line;
                            while (getline(u, line))
                                cout << line << endl;
                            pause();
                        }

                        else if (c == 3) {
                            ifstream cr("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/courses.txt");
                            string code, title, instr;
                            cout << "\n--- Courses ---\n";
                            while (cr >> code >> title >> instr)
                                cout << code << " " << title << " (" << instr << ")" << endl;
                            pause();
                        }

                    } while (c != 4);
                }

                // ================= INSTRUCTOR =================
                else if (role == "Instructor") {
                    int c;
                    do {
                        header("Instructor Dashboard");
                        cout <<
                        "1. Add Course\n"
                        "2. Remove Course\n"
                        "3. View My Courses\n"
                        "4. Add Assignment\n"
                        "5. Grade Assignment\n"
                        "6. Mark Attendance\n"
                        "7. Discussion Forum\n"
                        "8. Logout\nChoice: ";

                        cin >> c;
                        cin.ignore();

                        // ADD COURSE
                        if (c == 1) {
                            string code, title;
                            cout << "Course Code: ";
                            getline(cin, code);
                            cout << "Course Title: ";
                            getline(cin, title);

                            ofstream out("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/courses.txt", ios::app);
                            out << code << " " << title << " " << uid << endl;
                            out.close();

                            cout << "Course added successfully.\n";
                            pause();
                        }

                        // REMOVE COURSE (own only)
                        else if (c == 2) {
                            ifstream in("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/courses.txt");
                            ofstream temp("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/temp.txt");

                            string code, title, instr, removeCode;
                            bool found = false;

                            cout << "Enter Course Code to Remove: ";
                            getline(cin, removeCode);

                            while (in >> code >> title >> instr) {
                                if (!(code == removeCode && instr == uid))
                                    temp << code << " " << title << " " << instr << endl;
                                else
                                    found = true;
                            }

                            in.close();
                            temp.close();
                            remove("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/courses.txt");
                            rename("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/temp.txt", "C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/courses.txt");

                            if (found)
                                cout << "Course removed successfully.\n";
                            else
                                cout << "Course not found or unauthorized.\n";

                            pause();
                        }

                        // VIEW MY COURSES
                        else if (c == 3) {
                            ifstream in("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/courses.txt");
                            string code, title, instr;
                            cout << "\n--- My Courses ---\n";
                            while (in >> code >> title >> instr)
                                if (instr == uid)
                                    cout << code << " " << title << endl;
                            pause();
                        }

                        // ADD ASSIGNMENT
                        else if (c == 4) {
                            string code, title;
                            int marks;
                            cout << "Course Code: ";
                            getline(cin, code);
                            cout << "Assignment Title: ";
                            getline(cin, title);
                            cout << "Total Marks: ";
                            cin >> marks;
                            cin.ignore();

                            ofstream out("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/assignments.txt", ios::app);
                            out << code << " " << title << " " << marks << endl;
                            out.close();

                            cout << "Assignment added.\n";
                            pause();
                        }

                        // GRADE
                        else if (c == 5) {
                            string sid, code, feedback;
                            int marks;

                            cout << "Student ID: ";
                            getline(cin, sid);
                            cout << "Course Code: ";
                            getline(cin, code);
                            cout << "Marks: ";
                            cin >> marks;
                            cin.ignore();
                            cout << "Feedback: ";
                            getline(cin, feedback);

                            GradeBook::record(sid, code, marks);
                            Feedback::add(sid, code, feedback);
                            Notification::send(sid, "You have been graded in " + code);

                            cout << "Graded successfully.\n";
                            pause();
                        }

                        // ATTENDANCE
                        else if (c == 6) {
                            string sid, code;
                            int p;
                            cout << "Student ID: ";
                            getline(cin, sid);
                            cout << "Course Code: ";
                            getline(cin, code);
                            cout << "Present (1/0): ";
                            cin >> p;
                            cin.ignore();

                            Attendance::mark(sid, code, p);
                            pause();
                        }

                        // DISCUSSION
                        else if (c == 7) {
                            string code, msg;
                            cout << "Course Code: ";
                            getline(cin, code);
                            cout << "Message: ";
                            getline(cin, msg);

                            Discussion::postMessage(code, uid, msg);
                            pause();
                        }

                    } while (c != 8);
                }

                // ================= STUDENT =================
                else if (role == "Student") {
                    int c;
                    do {
                        header("Student Dashboard");
                        cout <<
                        "1. View Courses\n"
                        "2. View Grades\n"
                        "3. View Attendance\n"
                        "4. Discussion Forum\n"
                        "5. View Notifications\n"
                        "6. Logout\nChoice: ";

                        cin >> c;
                        cin.ignore();

                        if (c == 1) {
                            ifstream cr("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/courses.txt");
                            string code, title, instr;
                            cout << "\n--- Courses ---\n";
                            while (cr >> code >> title >> instr)
                                cout << code << " " << title << endl;
                            pause();
                        }

                        else if (c == 2) {
                            ifstream g("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/grades.txt");
                            string sid, code;
                            int marks;
                            cout << "\n--- Your Grades ---\n";
                            while (g >> sid >> code >> marks)
                                if (sid == uid)
                                    cout << code << " : " << marks << endl;
                            pause();
                        }

                        else if (c == 3) {
                            Attendance::view(uid);
                            pause();
                        }

                        else if (c == 4) {
                            string code;
                            cout << "Course Code: ";
                            getline(cin, code);
                            Discussion::viewMessages(code);
                            pause();
                        }

                        else if (c == 5) {
                            Notification::view(uid);
                            pause();
                        }

                    } while (c != 6);
                }

                break;
            }
        }

        fin.close();
        if (!found) {
            cout << "Invalid credentials.\n";
            pause();
        }
    }
}
