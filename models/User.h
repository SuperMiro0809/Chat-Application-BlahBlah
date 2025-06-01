#pragma once

#include "../utils/String.h"

class User {
    String username;

    public:
        User(const String& username);

        const String& getUsername() const;
};