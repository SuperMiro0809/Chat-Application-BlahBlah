#include "CreateGroupCommand.h"

#include "../models/User.h"
#include "../core/Constants.h"
#include "../services/UsersDatabase.h"
#include "../services/ChatsDatabase.h"
#include "../services/ChatParticipantsDatabase.h"

CreateGroupCommand::CreateGroupCommand(const String& groupName, const Vector<String>& participants):
    groupName(groupName),
    participants(participants) {}

void CreateGroupCommand::execute(System& system) const {
    const User* currUser = system.getCurrentUser();

    if (!currUser) {
        throw std::logic_error("Command forbidden!");
    }

    ChatsDatabase chatsDb(CHATS_DB_NAME);

    unsigned int chatId = chatsDb.addNewChat(groupName, CHAT_GROUP);

    // add participants
    UsersDatabase usersDb(USERS_DB_NAME);
    ChatParticipantsDatabase chatParticipantsDb(PARTICIPANTS_DB_NAME);

    for (size_t i = 0; i < participants.getSize(); i++) {
        User* user = usersDb.getUser(participants[i]);

        if (user->getId() == currUser->getId()) { // creator will be added later as admin
            continue;
        }

        if (user) {
            chatParticipantsDb.addNewParticipant(chatId, user->getId(), PARTICIPANT_MEMBER);
            delete user;
        } else {
            std::cout << participants[i] << " not found!" << std::endl;
        }
    }

    // add group admin - current user
    chatParticipantsDb.addNewParticipant(chatId, currUser->getId(), PARTICIPANT_ADMIN);

    std::cout << "Group created successfully" << std::endl;
}