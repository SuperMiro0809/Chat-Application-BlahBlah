#include "System.h"

System::System() {
    currentUser = nullptr;
}

const User* System::getCurrentUser() const {
    return currentUser;
}

User* System::getCurrentUser() {
    return currentUser;
}

void System::setCurrentUser(User* user) {
    currentUser = user;
}

void System::logout() {
    delete currentUser;
    currentUser = nullptr;
}