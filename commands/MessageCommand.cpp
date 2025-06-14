#include "MessageCommand.h"

#include "../services/UsersDatabase.h"
#include "../services/ChatsDatabase.h"
#include "../services/ChatParticipantsDatabase.h"
#include "../services/ChatMessagesDatabase.h"
#include "../core/Constants.h"

MessageCommand::MessageCommand(const String& username, const String& message): username(username), message(message) {}

void MessageCommand::execute(System& system) const {
    const User* currUser = system.getCurrentUser();

    if (!currUser) {
        throw std::logic_error("Command forbidden!");
    }

    if (currUser->getUsername() == username) {
        throw std::logic_error("You can not send message to yourself!");
    }

    UsersDatabase usersDb(USERS_DB_NAME);
    User* user = usersDb.getUser(username);

    if (!user) {
        throw std::logic_error("User with this username does not exist!");
    }

    ChatsDatabase chatsDb(CHATS_DB_NAME);
    const Chat* chat = chatsDb.findIndividualChat(currUser->getId(), user->getId());

    if (!chat) {
        std::cout << "Chat with this user does not exist. Create? (y/n): ";

        char answer;
        while (true) {
            std::cin >> answer;

            if (answer == 'y') {
                String chatName = currUser->getUsername() + "-" + username;
                unsigned int chatId = chatsDb.addNewChat(chatName, CHAT_INDIVIDUAL);

                // add participants
                ChatParticipantsDatabase chatParticipantsDb(PARTICIPANTS_DB_NAME);
                chatParticipantsDb.addNewParticipant(chatId, currUser->getId(), PARTICIPANT_MEMBER);
                chatParticipantsDb.addNewParticipant(chatId, user->getId(), PARTICIPANT_MEMBER);

                // send message
                ChatMessagesDatabase chatMessagesDb(MESSAGES_DB_NAME);
                chatMessagesDb.addNewMessage(chatId, currUser->getId(), message);

                std::cout << "Chat created successfully. Message sent successfully." << std::endl;
                break;
            } else if (answer == 'n') {
                std::cout << "Chat will not be created." << std::endl;
                break;
            } else {
                std::cout << "Unrecognized operation. Please try again" << std::endl;
            }
        }
    } else {
        unsigned int chatId = chat->getId();

        // send message
        ChatMessagesDatabase chatMessagesDb(MESSAGES_DB_NAME);
        chatMessagesDb.addNewMessage(chatId, currUser->getId(), message);

         std::cout << "Message sent successfully." << std::endl;
    }

    delete user;
    delete chat;
}