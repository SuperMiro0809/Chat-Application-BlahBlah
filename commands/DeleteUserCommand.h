#pragma once

#include "Command.h"
#include "../utils/String.h"

class DeleteUserCommand: public Command {
    String username;

    public:
        DeleteUserCommand(const String& username);

        void execute(System& system) const override;

        bool isAllowed(System& system) const override;
};