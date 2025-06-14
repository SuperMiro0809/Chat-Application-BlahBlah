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
#include "AddToGroupCommand.h"
#include "GroupStatsCommand.h"
#include "ViewAllChatsCommand.h"
#include "LeaveGroupCommand.h"
#include "KickFromGroupCommand.h"
#include "SetGroupAdminCommand.h"
#include "DeleteUserCommand.h"

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
    } else if (cmd == "add-to-group") {
        unsigned int chatId;
        String username;
        ss >> std::ws >> chatId;
        ss >> std::ws >> username;

        return new AddToGroupCommand(chatId, username);
    } else if (cmd == "group-stats") {
        unsigned int chatId;
        ss >> std::ws >> chatId;

        return new GroupStatsCommand(chatId);
    } else if (cmd == "view-all-chats") {
        return new ViewAllChatsCommand();
    } else if (cmd == "leave-group") {
        unsigned int chatId;
        ss >> std::ws >> chatId;

        return new LeaveGroupCommand(chatId);
    } else if (cmd == "kick-from-group") {
        unsigned int chatId;
        String username;
        ss >> std::ws >> chatId;
        ss >> std::ws >> username;

        return new KickFromGroupCommand(chatId, username);
    } else if (cmd == "set-group-admin") {
        unsigned int chatId;
        String username;
        ss >> std::ws >> chatId;
        ss >> std::ws >> username;

        return new SetGroupAdminCommand(chatId, username);
    } else if (cmd == "delete-user") {
        String username;
        ss >> std::ws >> username;

        return new DeleteUserCommand(username);
    }

    return nullptr;
}