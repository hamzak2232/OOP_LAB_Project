#include "User.h"
User::User(string i, string n, string p, string r)
: id(i), name(n), password(p), role(r) {}
string User::getID() { return id; }
string User::getRole() { return role; }
bool User::authenticate(string p) { return password == p; }