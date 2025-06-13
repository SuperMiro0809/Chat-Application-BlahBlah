#pragma once

#include "../utils/String.h"

enum class UserRole {
    User,
    Admin
};

class User {
    unsigned int id;
    String username;
    String password;

    public:
        User(unsigned int id, const String& username, const String& password);

        User(unsigned int id, const char* username, const char* password);

        unsigned int getId() const;

        const String& getUsername() const;

        bool doPasswordsMatch(const String& checkPass) const;

        virtual UserRole getRole() const;

        virtual ~User() = default;
};