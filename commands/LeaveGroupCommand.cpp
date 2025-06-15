#include "LeaveGroupCommand.h"

#include "../services/ChatParticipantsDatabase.h"

LeaveGroupCommand::LeaveGroupCommand(unsigned int chatId): chatId(chatId) {}

void LeaveGroupCommand::execute(System& system) const {
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

    Vector<ChatParticipant>& participants = *selectedChat->getParticipants();

    bool isAdmin = false;
    size_t adminCount = 0;

    for (size_t i = 0; i < participants.getSize(); ++i) {
        const ChatParticipant& p = participants[i];

        if (p.getType() == ParticipantTypes::ADMIN) {
            adminCount++;
        }

        if (p.getUserId() == currUser->getId()) {
            if (p.getType() == ParticipantTypes::ADMIN) {
                isAdmin = true;
            }
        }
    }

    if (isAdmin && adminCount == 1) {
        throw std::logic_error("You are the only admin. Assign another admin before leaving.");
    }

    ChatParticipantsDatabase chatParticipantsDb(PARTICIPANTS_DB_NAME);
    chatParticipantsDb.removeParticipant(chatId, currUser->getId());

    std::cout << "You have successfully left " << selectedChat->getName() << "." << std::endl;

    for (size_t i = 0; i < chats.getSize(); i++) {
        if (chats[i].getId() == chatId) {
            chats.removeAt(i);
            break;
        }
    }
}