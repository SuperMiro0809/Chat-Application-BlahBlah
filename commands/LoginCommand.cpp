#include "LoginCommand.h"

#include <stdexcept>
#include "../services/UsersDatabase.h"

LoginCommand::LoginCommand(const String& username, const String& password):
    username(username),
    password(password) {}

void LoginCommand::execute(System& system) {
    const User* currUser = system.getCurrentUser();

    if (currUser) {
        throw std::logic_error("Command forbidden!");
    }

    UsersDatabase usersDb("../users");
    User* user = usersDb.getUser(username);

    if (!user) {
        std::cout << "Account not found. Create? (y/n): ";

        char answer;
        while (true) {
            std::cin >> answer;

            if (answer == 'y') {
                usersDb.addNewUser(username, password, "User");
                std::cout << "Account created successfully. Now you can login" << std::endl;
                break;
            } else if (answer == 'n') {
                std::cout << "Account will not be created." << std::endl;
                break;
            } else {
                std::cout << "Unrecognized operation. Please try again" << std::endl;
            }
        }
    } else {
        if (user->doPasswordsMatch(password)) {
            system.setCurrentUser(user);
            std::cout << "Welcome, " << username << "!" << std::endl;
        } else {
            delete user;
            std::cout << "Incorrect password. Plase try again" << std::endl;
        }
    }
}