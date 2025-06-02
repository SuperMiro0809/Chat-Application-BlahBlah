#pragma once

#include "../utils/String.h"

class User {
    String username;
    String password;

    public:
        User(const String& username, const String& password);

        User(const char* username, const char* password);

        const String& getUsername() const;

        bool doPasswordsMatch(const String& checkPass) const;
};