#include "LogoutCommand.h"

#include <stdexcept>
#include "../models/User.h"

void LogoutCommand::execute(System& system) const {
    const User* currUser = system.getCurrentUser();

    if (!currUser) {
        throw std::logic_error("Command forbidden!");
    }

    std::cout << "Goodbye, " << currUser->getUsername() << "!" << std::endl;
    system.logout();
}
