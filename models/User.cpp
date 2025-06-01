#include "User.h"

User::User(const String& username): username(username) {}

const String& User::getUsername() const {
    return username;
}