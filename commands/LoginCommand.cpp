#include "LoginCommand.h"

#include "../services/UsersDatabase.h"

LoginCommand::LoginCommand(const String& username, const String& password):
    username(username),
    password(password) {}

void LoginCommand::execute(System& system) {
    const User* currUser = system.getCurrentUser();

    if (currUser) {
        throw std::logic_error("Command forbidden!");
    }

    UsersDatabase usersDb(String("../users"));

    User* user = usersDb.getUser(username);

    if (!user) {
        std::cout << "Account not found. Create? (y/n): ";
        char answer;
        std::cin >> answer;

        if (answer == 'y') {
            usersDb.addNewUser(username, password, "User");
            std::cout << "Account created successfully. Now you can login" << std::endl;
        }
    } else {
        std::cout << "Account found" << std::endl;
    }
}