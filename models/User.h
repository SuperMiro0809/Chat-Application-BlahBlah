#pragma once

#include "../utils/String.h"

enum class UserRole {
    User,
    Admin
};

class User {
    String username;
    String password;

    public:
        User(const String& username, const String& password);

        User(const char* username, const char* password);

        const String& getUsername() const;

        bool doPasswordsMatch(const String& checkPass) const;

        virtual UserRole getRole() const;

        virtual ~User() = default;
};