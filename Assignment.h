#ifndef BAHRIA_LMS_ASSIGNMENT_H
#define BAHRIA_LMS_ASSIGNMENT_H

#include <string>

class Assignment {
private:
    int id;
    std::string title;
    std::string description;
    int maxMarks;
    std::string deadline;

public:
    Assignment(int id, std::string title, std::string desc, int marks, std::string date)
        : id(id), title(title), description(desc), maxMarks(marks), deadline(date) {}

    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getDescription() const { return description; }
    int getMaxMarks() const { return maxMarks; }
    std::string getDeadline() const { return deadline; }
};

#endif //BAHRIA_LMS_ASSIGNMENT_H
