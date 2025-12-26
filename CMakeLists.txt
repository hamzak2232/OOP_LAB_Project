cmake_minimum_required(VERSION 3.20)
project(Bahria_LMS)

set(CMAKE_CXX_STANDARD 20)

add_executable(Bahria_LMS 
    main.cpp
    LMS.cpp
    Course.cpp
    Student.cpp
    Faculty.cpp
    Admin.cpp
    WebServer.cpp
    Database.cpp
    Attendance.cpp
    Grade.cpp
    SeatingPlan.cpp
    FeeChallan.cpp
    # Headers are not strictly needed here but good for IDEs
    LMS.h
    User.h
    Student.h
    Faculty.h
    Admin.h
    Course.h
    Assignment.h
    WebServer.h
    Database.h
    Attendance.h
    Grade.h
    SeatingPlan.h
    FeeChallan.h
    UIUtils.h
)

target_link_libraries(Bahria_LMS ws2_32)

# Copy www folder to build directory
add_custom_command(TARGET Bahria_LMS POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/www
        ${CMAKE_BINARY_DIR}/www)
