#include "GroupStatsCommand.h"

GroupStatsCommand::GroupStatsCommand(unsigned int chatId): chatId(chatId) {}

void GroupStatsCommand::execute(System& system) const {
    User* currUser = system.getCurrentUser();

    if (!currUser) {
        throw std::logic_error("Command forbidden!");
    }

    if (!currUser->getAreChatsLoaded()) {
        currUser->loadCriteriaChats();
    }

    Vector<Chat>& chats = currUser->getChats();
    Chat* selectedChat = nullptr;

    for (size_t i = 0; i < chats.getSize(); i++) {
        if (chats[i].getId() == chatId) {
            selectedChat = &chats[i];
            break;
        }
    }

    if (!selectedChat) {
        throw std::logic_error("Chat not found!");
    }

    if (selectedChat->getChatType() != ChatType::GROUP) {
        throw std::logic_error("Chat is not a group!");
    }

    if (!selectedChat->getAreParticipantsLoaded()) {
        selectedChat->loadParticipants();
    }

    if (!selectedChat->getAreMessagesLoaded()) {
        selectedChat->loadMessages();
    }

    Vector<ChatParticipant>& participants = *selectedChat->getParticipants();
    Vector<ChatMessage>& messages = *selectedChat->getMessages();

    std::cout << selectedChat->getName() << ": " << participants.getSize() << " members, "
              << messages.getSize() << " messages." << std::endl;
}