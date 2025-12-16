cmake_minimum_required(VERSION 4.0)
project(Bahria_LMS_Clone)

set(CMAKE_CXX_STANDARD 20)

add_executable(Bahria_LMS_Clone main.cpp
        User.cpp
        Student.cpp
        Instructor.cpp
        Admin.cpp
        Course.cpp
        Assignment.cpp
        Submission.cpp
        LMS.cpp
        User.h
        Student.h
        Instructor.h
        Admin.h
        Course.h
        Assignment.h
        Submission.h
        LMS.h
        GradeBook.cpp
        GradeBook.h
        Utils.h
        Notification.cpp
        Notification.h
        Discussion.cpp
        Discussion.h
        Attendance.cpp
        Attendance.h
        Feedback.cpp
        Feedback.h
)
