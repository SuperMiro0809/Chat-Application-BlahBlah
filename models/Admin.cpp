#include "Admin.h"

Admin::Admin(unsigned int id, const String& username, const String& password): User(username, password) {
    this->id = id;
}

Admin::Admin(unsigned int id, const char* username, const char* password): User(username, password) {
    this->id = id;
}
