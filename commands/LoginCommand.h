#pragma once

#include "Command.h"
#include "../utils/String.h"

class LoginCommand: public Command {
    String username;
    String password;

    public:
        LoginCommand(const String& username, const String& password);

        void execute(System& system) const override;
};