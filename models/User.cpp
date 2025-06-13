#include "User.h"

User::User(unsigned int id, const String& username, const String& password): id(id), username(username), password(password) {}

User::User(unsigned int id, const char* username, const char* password): id(id), username(username), password(password) {}

unsigned int User::getId() const {
    return id;
}

const String& User::getUsername() const {
    return username;
}

bool User::doPasswordsMatch(const String& checkPass) const {
    return password == checkPass;
}

UserRole User::getRole() const{
    return UserRole::User;
}