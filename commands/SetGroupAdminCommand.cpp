#include "SetGroupAdminCommand.h"

#include "../services/UsersDatabase.h"
#include "../services/ChatParticipantsDatabase.h"

SetGroupAdminCommand::SetGroupAdminCommand(unsigned int chatId, const String& username): chatId(chatId), username(username) {}

void SetGroupAdminCommand::execute(System& system) const {
     User* currUser = system.getCurrentUser();

    if (!currUser) {
        throw std::logic_error("Command forbidden!");
    }

    if (!currUser->getAreChatsLoaded()) {
        currUser->loadCriteriaChats();
    }

    UsersDatabase usersDb(USERS_DB_NAME);
    User* newAdmin = usersDb.getUser(username);

    if (!newAdmin) {
        throw std::logic_error("User does not exist!");
    }

    unsigned int newAdminId = newAdmin->getId();
    delete newAdmin;

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

    for (size_t i = 0; i < participants.getSize(); i++) {
        const ChatParticipant& p = participants[i];

        if (p.getUserId() == currUser->getId()) {
            if (p.getType() == ParticipantTypes::ADMIN) {
                isAdmin = true;
            } else {
                break;
            }
        }

        if (p.getUserId() == newAdminId) {
            isNewPart = true;
        }
    }

    if (!isAdmin) {
        throw std::logic_error("You do not have permissions to make user admin!");
    }

    if (!isNewPart) {
        throw std::logic_error("User is not a member of this group!");
    }

    bool updated = false;

    for (size_t i = 0; i < participants.getSize(); i++) {
        ChatParticipant& p = participants[i];

        if (p.getUserId() == newAdminId) {
            if (p.getType() != ParticipantTypes::ADMIN) {
                p.setType(ParticipantTypes::ADMIN);
                updated = true;
            }
            break;
        }
    }

    if (!updated) {
        std::cout << "User is already an admin in this group." << std::endl;
        return;
    }

    ChatParticipantsDatabase participantsDb(PARTICIPANTS_DB_NAME);
    participantsDb.updateParticipantRole(selectedChat->getId(), newAdminId, PARTICIPANT_ADMIN);

    std::cout << username << " is now admin of " << selectedChat->getName() << "!" << std::endl;
}