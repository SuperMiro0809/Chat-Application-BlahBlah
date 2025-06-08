#pragma once

#include "Command.h"
#include "../utils/String.h"

class CreateAccountCommand: public Command {
    String username;
    String password;

    public:
        CreateAccountCommand(const String& username, const String& password);

        void execute(System& system) const override;
};