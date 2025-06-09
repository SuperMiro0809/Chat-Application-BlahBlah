#include "User.h"

User::User(const String& username, const String& password): username(username), password(password) {}

User::User(const char* username, const char* password): username(username), password(password) {}

const String& User::getUsername() const {
    return username;
}

bool User::doPasswordsMatch(const String& checkPass) const {
    return password == checkPass;
}

UserRole User::getRole() const{
    return UserRole::User;
}