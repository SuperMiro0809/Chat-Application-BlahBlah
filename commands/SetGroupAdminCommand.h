#pragma once

#include "Command.h"

class SetGroupAdminCommand: public Command {
    unsigned int chatId;
    String username;

    public:
        SetGroupAdminCommand(unsigned int chatId, const String& username);

        void execute(System& system) const override;
};