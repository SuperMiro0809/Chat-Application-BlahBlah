#include "DeleteUserCommand.h"

#include "../services/UsersDatabase.h"
#include "../services/ChatParticipantsDatabase.h"
#include "../services/ChatMessagesDatabase.h"

DeleteUserCommand::DeleteUserCommand(const String& username): username(username) {}

void DeleteUserCommand::execute(System& system) const {
    const User* currUser = system.getCurrentUser();

    if (!currUser) {
        throw std::logic_error("Command forbidden!");
    }

    UsersDatabase usersDb(USERS_DB_NAME);
    User* userToDelete = usersDb.getUser(username);

    if (!userToDelete) {
        throw std::logic_error("User does not exist.");
    }

    unsigned int userId = userToDelete->getId();
    delete userToDelete;

    if (userId == currUser->getId()) {
        throw std::logic_error("You can not delete youself.");
    }

    ChatParticipantsDatabase participantsDb(PARTICIPANTS_DB_NAME);
    participantsDb.removeByUser(userId);

    ChatMessagesDatabase messagesDb(MESSAGES_DB_NAME);
    messagesDb.removeByUser(userId);

    usersDb.removeUserById(userId);

    std::cout << "User deleted successfully!" << std::endl;
}

bool DeleteUserCommand::isAllowed(System& system) const {
    const User* currUser = system.getCurrentUser();

    return currUser->getRole() == UserRole::Admin;
}