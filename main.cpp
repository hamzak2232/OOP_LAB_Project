#include "LMS.h"
#include "WebServer.h"
#include <iostream>

int main() {
    LMS lms;
    
    std::cout << "Select Mode:\n";
    std::cout << "1. Console Application\n";
    std::cout << "2. Web Server GUI (http://localhost:8080)\n";
    int choice;
    std::cin >> choice;

    if (choice == 2) {
        WebServer server(&lms);
        server.start();
    } else {
        lms.run();
    }
    
    return 0;
}