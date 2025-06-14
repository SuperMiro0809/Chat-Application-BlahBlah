#include "SelectChatCommand.h"

#include "../services/ChatsDatabase.h"
#include "../core/Constants.h"

SelectChatCommand::SelectChatCommand(unsigned int chatId): chatId(chatId) {}

void SelectChatCommand::execute(System& system) const {
    User* currUser = system.getCurrentUser();

    if (!currUser) {
        throw std::logic_error("Command forbidden!");
    }

    if (!currUser->getAreChatsLoaded()) {
        currUser->loadCriteriaChats();
    }

    Vector<Chat> chats = currUser->getChats();
    bool isChatFound = false;

    for (size_t i = 0; i < chats.getSize(); i++) {
        Chat chat = chats[i];
        if (chat.getId() == chatId) {
            isChatFound = true;

            Vector<ChatMessage> messages;
            messages.loadFromFileByCriteria(MESSAGES_DB_NAME, chatId);

            for (size_t j = 0; j < messages.getSize(); j++) {
                messages[j].print();
            }
        }
    }

    if (!isChatFound) {
        throw std::logic_error("Chat not found!");
    }
}