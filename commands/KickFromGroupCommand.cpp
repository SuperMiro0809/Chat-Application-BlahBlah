#include "KickFromGroupCommand.h"

#include "../services/UsersDatabase.h"
#include "../services/ChatParticipantsDatabase.h"

KickFromGroupCommand::KickFromGroupCommand(unsigned int chatId, const String& username): chatId(chatId), username(username) {}

void KickFromGroupCommand::execute(System& system) const {
    User* currUser = system.getCurrentUser();

    if (!currUser) {
        throw std::logic_error("Command forbidden!");
    }

    if (!currUser->getAreChatsLoaded()) {
        currUser->loadCriteriaChats();
    }

    UsersDatabase usersDb(USERS_DB_NAME);
    User* oldParticipant = usersDb.getUser(username);

    if (!oldParticipant) {
        throw std::logic_error("User does not exist!");
    }

    unsigned int oldParticipantId = oldParticipant->getId();
    delete oldParticipant;

    if (oldParticipantId == currUser->getId()) {
        throw std::logic_error("You can not kick yourself!");
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
    bool isNewPart = false;
    for (size_t i = 0; i < participants.getSize(); ++i) {
        const ChatParticipant& p = participants[i];

        if (p.getUserId() == oldParticipantId) {
            isNewPart = true;
        }

        if (p.getUserId() == currUser->getId()) {
            if (p.getType() == ParticipantTypes::ADMIN) {
                isAdmin = true;
            } else {
              break;
            }
        }
    }

    if (!isAdmin) {
        throw std::logic_error("You do not have permissions to kick users!");
    }

    if (!isNewPart) {
        throw std::logic_error("User is not a member of this group!");
    }


    ChatParticipantsDatabase chatParticipantsDb(PARTICIPANTS_DB_NAME);
    chatParticipantsDb.removeParticipant(chatId, oldParticipantId);

    for (size_t i = 0; i < participants.getSize(); i++) {
        if (participants[i].getUserId() == oldParticipantId) {
            participants.removeAt(i);
            break;
        }
    }

    std::cout << username << " kicked from " << selectedChat->getName() << " successfully." << std::endl;
}