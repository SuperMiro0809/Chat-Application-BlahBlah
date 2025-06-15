#pragma once

#include "Database.h"
#include "../models/User.h"

class UsersDatabase: public Database {
    void writeUserToTextFile(unsigned int id, const String& username, const String& password, const char* role) const;

    void writeUserToBinaryFile(unsigned int id, const String& username, const String& password, const char* role) const;

    public:
        UsersDatabase(const char* dbName);

        void addNewUser(const String& username, const String& password, const char* role) const;

        User* getUser(const String& username) const;

        User* getById(unsigned int id) const;

        bool isUsernameTaken(const String& username) const;

        void removeUserById(unsigned int userId) const;
};