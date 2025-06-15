#include "CommandFactory.h"

#include <sstream>
#include "../utils/Vector.hpp"
#include "LoginCommand.h"
#include "LogoutCommand.h"
#include "CreateAccountCommand.h"
#include "FileModeCommand.h"
#include "MessageCommand.h"
#include "SelectChatCommand.h"
#include "CreateGroupCommand.h"
#include "ViewChatsCommand.h"

Command* CommandFactory::create(const String& input) {
    std::stringstream ss(input.getElements());
    String cmd;
    ss >> std::ws >> cmd;

    if (cmd == "login") {
        String username, password;
        ss >> std::ws >> username;
        ss >> std::ws >> password;

        return new LoginCommand(username, password);
    } else if (cmd == "logout") {
        return new LogoutCommand();
    } else if (cmd == "create-account") {
        String username, password;
        ss >> std::ws >> username;
        ss >> std::ws >> password;

        return new CreateAccountCommand(username, password);
    } else if (cmd == "file-mode") {
        return new FileModeCommand();
    } else if (cmd == "message") {
        String username, message;
        ss >> std::ws >> username;
        ss >> std::ws;
        getline(ss, message);

        return new MessageCommand(username, message);
    } else if (cmd == "select-chat") {
        unsigned int chatId;
        ss >> std::ws >> chatId;

        return new SelectChatCommand(chatId);
    } else if (cmd == "create-group") {
        String groupName;
        ss >> std::ws >> groupName;

        Vector<String> participants;
        String username;

        while (ss >> std::ws >> username) {
            participants.add(username);
        }

        return new CreateGroupCommand(groupName, participants);
    } else if (cmd == "view-chats") {
        return new ViewChatsCommand();
    }

    return nullptr;
}