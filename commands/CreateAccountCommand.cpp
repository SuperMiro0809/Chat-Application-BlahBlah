#include "CreateAccountCommand.h"

#include "../services/UsersDatabase.h"

CreateAccountCommand::CreateAccountCommand(const String& username, const String& password):
    username(username),
    password(password) {}

void CreateAccountCommand::execute(System& system) const {
    const User* currUser = system.getCurrentUser();

    if (currUser) {
        throw std::logic_error("Command forbidden!");
    }

    UsersDatabase usersDb("../users");
    usersDb.addNewUser(username, password, "User");
    std::cout << "Account created successfully. Now you can login" << std::endl;
}