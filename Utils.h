#ifndef UTILS_H
#define UTILS_H
#include <iostream>
using namespace std;


inline void header(string title) {
    cout << "========================================";
    cout << " " << title << " ";
    cout << "========================================" << endl;
    }


inline void pause() {
    cout << "Press Enter to continue...";
    cin.ignore(); cin.get();
}
#endif