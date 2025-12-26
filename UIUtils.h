#ifndef BAHRIA_LMS_UIUTILS_H
#define BAHRIA_LMS_UIUTILS_H

#include <iostream>
#include <string>
#include <cstdlib>

class UIUtils {
public:
    static void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    static void printHeader(const std::string& title) {
        clearScreen();
        std::cout << "========================================\n";
        std::cout << "      BAHRIA UNIVERSITY LMS           \n";
        std::cout << "========================================\n";
        std::cout << ">>> " << title << "\n";
        std::cout << "----------------------------------------\n";
    }

    static void printFooter() {
        std::cout << "----------------------------------------\n";
        std::cout << "(c) 2025 Bahria University Clone\n";
    }

    static void pause() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }
    
    static int getIntInput(const std::string& prompt) {
        int x;
        std::cout << prompt;
        while(!(std::cin >> x)){
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input. " << prompt;
        }
        std::cin.ignore(1000, '\n'); // clear buffer
        return x;
    }

    static float getFloatInput(const std::string& prompt) {
        float x;
        std::cout << prompt;
        while(!(std::cin >> x)){
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input. " << prompt;
        }
        std::cin.ignore(1000, '\n'); // clear buffer
        return x;
    }
    
    static std::string getStringInput(const std::string& prompt) {
        std::string s;
        std::cout << prompt;
        // logic to handle spaces if needed, but for ID/Pass usually no spaces.
        // If we want lines:
        // std::getline(std::cin, s);
        // But mixing cin >> and getline is tricky.
        std::cin >> s;
        return s;
    }
};

#endif //BAHRIA_LMS_UIUTILS_H
