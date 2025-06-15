#include "SelectChatCommand.h"

#include "../services/ChatsDatabase.h"
#include "../services/ChatMessagesDatabase.h"
#include "../core/Constants.h"

static void printMessages(const Vector<ChatMessage>& messages) {
    for (size_t i = 0; i < messages.getSize(); ++i) {
        messages[i].print();
    }
}

SelectChatCommand::SelectChatCommand(unsigned int chatId): chatId(chatId) {}

void SelectChatCommand::execute(System& system) const {
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

    if (!selectedChat->getAreMessagesLoaded()) {
        selectedChat->loadMessages();
    }

    Vector<ChatMessage>& messages = *selectedChat->getMessages();
    printMessages(messages);

    std::cout << "Enter message (or type 'no' to cancel): " << std::endl;
    String message;

    ChatMessagesDatabase chatMessagesDb(MESSAGES_DB_NAME);

    while (true) {
        std::cout << "> ";
        std::cin >> std::ws;
        getline(std::cin, message);

        if (message == "no") {
            std::cout << "Message input cancelled." << std::endl;
            break;
        }

        ChatMessage newMsg = chatMessagesDb.addNewMessage(chatId, currUser->getId(), message);
        std::cout << "Message sent." << std::endl;
        messages.add(newMsg);

        printMessages(messages);
    }
}