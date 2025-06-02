#include "CommandFactory.h"

#include <sstream>
#include "LoginCommand.h"

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

    }

    return nullptr;
}