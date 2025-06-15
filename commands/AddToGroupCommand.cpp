#include "AddToGroupCommand.h"

#include "../services/UsersDatabase.h"
#include "../services/ChatParticipantsDatabase.h"

AddToGroupCommand::AddToGroupCommand(unsigned int chatId, const String& username): chatId(chatId), username(username) {}

void AddToGroupCommand::execute(System& system) const {
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

    UsersDatabase usersDb(USERS_DB_NAME);
    User* newParticipant = usersDb.getUser(username);

    if (!newParticipant) {
        throw std::logic_error("User does not exist!");
    }

    bool isCurrUserPart = false;
    bool isCurrUserAdmin = false;
    bool isNewUserPart = false;

    for (size_t i = 0; i < participants.getSize(); i++) {
        const ChatParticipant& participant = participants[i];
        if (participant.getUserId() == currUser->getId()) {
            isCurrUserPart = true;

            if (participant.getType() == ParticipantTypes::ADMIN) {
                isCurrUserAdmin = true;
            } else {
                break;
            }
        }

        if (participant.getUserId() == newParticipant->getId()) {
            isNewUserPart = true;
            break;
        }
    }

    if (isNewUserPart) {
        delete newParticipant;
        throw std::logic_error("User is already part of this group!");
    }

    if (!isCurrUserPart) {
        delete newParticipant;
        throw std::logic_error("You are not part of this group!");
    }

    if (!isCurrUserAdmin) {
        delete newParticipant;
        throw std::logic_error("You do not have permissions to add users to this group!");
    }

    ChatParticipantsDatabase chatParticipantsDb(PARTICIPANTS_DB_NAME);
    ChatParticipant participant = chatParticipantsDb.addNewParticipant(chatId, newParticipant->getId(), PARTICIPANT_MEMBER);
    delete newParticipant;
    participants.add(participant);

    std::cout << username << " added to group " << selectedChat->getName() << " successfully!" << std::endl;
}