#pragma once

#include "Database.h"
#include "../models/User.h"

class UsersDatabase: public Database {
    void writeUserToTextFile(unsigned int id, const String& username, const String& password, const char* role) const;

    void writeUserToBinaryFile(unsigned int id, const String& username, const String& password, const char* role) const;

    public:
        UsersDatabase(const String& dbName);

        void addNewUser(const String& username, const String& password, const char* role) const;

        User* getUser(const String& username) const;

        bool isUsernameTaken(const String& username) const;
};