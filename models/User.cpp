#include "User.h"

#include "../core/Constants.h"

User::User(unsigned int id, const String& username, const String& password): id(id), username(username), password(password) {}

User::User(unsigned int id, const char* username, const char* password): id(id), username(username), password(password) {}

unsigned int User::getId() const {
    return id;
}

const String& User::getUsername() const {
    return username;
}

bool User::getAreChatsLoaded() const {
    return areChatsLoaded;
}

bool User::doPasswordsMatch(const String& checkPass) const {
    return password == checkPass;
}

void User::loadCriteriaChats() {
    chats.loadFromFileByCriteria(CHATS_DB_NAME, id);
    areChatsLoaded = true;
}

const Vector<Chat>& User::getChats() const {
    return chats;
}

UserRole User::getRole() const{
    return UserRole::User;
}