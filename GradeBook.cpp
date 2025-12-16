#include "GradeBook.h"
#include <filesystem>

void GradeBook::record(string student, string course, int marks) {
    std::filesystem::create_directory("data");

    ofstream out("C:/Users/hamza/CLionProjects/Bahria_LMS_Clone/data/grades.txt", ios::app);
    if (!out) {
        cout << "Error opening grades file.\n";
        return;
    }
    out << student << "," << course << "," << marks << endl;
    out.close();
}
