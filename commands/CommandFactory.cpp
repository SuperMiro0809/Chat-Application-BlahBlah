#include "CommandFactory.h"

#include <sstream>
#include "LoginCommand.h"
#include "LogoutCommand.h"
#include "CreateAccountCommand.h"
#include "FileModeCommand.h"

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
    }

    return nullptr;
}