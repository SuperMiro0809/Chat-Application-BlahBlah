#include "FileModeCommand.h"

#include "../models/User.h"
#include "../core/SystemSettings.h"

void FileModeCommand::execute(System& system) const {
     SystemSettings systemSettings = SystemSettings::getInstance();
     String token;

     std::cout << "Current file mode: " << systemSettings.getFileMode() << std::endl;
     std::cout << "Change to <text | binary | no>: ";

     while (true) {
          std::cin >> std::ws >> token;

          if (token == "text") {
               systemSettings.setFileMode(FileMode::TEXT);
               break;
          } else if (token == "binary") {
               systemSettings.setFileMode(FileMode::BINARY);
               break;
          } else if (token == "no") {
               break;
          } else {
               std::cout << "Unrecognized file mode. Choose one from <text | binary | no>" << std::endl;
          }
     }
}

bool FileModeCommand::isAllowed(System& system) const {
     const User* currUser = system.getCurrentUser();

     return currUser->getRole() == UserRole::Admin;
}