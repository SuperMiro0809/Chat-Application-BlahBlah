#pragma once

#include "User.h"

class Admin: public User {
    unsigned int id;

    public:
        Admin(unsigned int id, const String& username, const String& password);

        Admin(unsigned int id, const char* username, const char* password);
};