#include "ViewChatsCommand.h"

void ViewChatsCommand::execute(System& system) const {
    User* currUser = system.getCurrentUser();

    if (!currUser) {
        throw std::logic_error("Command forbidden!");
    }

    if (!currUser->getAreChatsLoaded()) {
        currUser->loadCriteriaChats();
    }

    Vector<Chat>& chats = currUser->getChats();

    for (size_t i = 0; i < chats.getSize(); i++) {
        chats[i].print();
    }
}