#include "ViewAllChatsCommand.h"

#include "../utils/Vector.hpp"

void ViewAllChatsCommand::execute(System& system) const {
    User* currUser = system.getCurrentUser();

    if (!currUser) {
        throw std::logic_error("Command forbidden!");
    }

    Vector<Chat> chats;
    chats.loadFromFile(CHATS_DB_NAME);

    for (size_t i = 0; i < chats.getSize(); i++) {
        chats[i].print();
    }
}

bool ViewAllChatsCommand::isAllowed(System& system) const {
    const User* currUser = system.getCurrentUser();

    return currUser->getRole() == UserRole::Admin;
}