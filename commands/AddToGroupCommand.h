#pragma once

#include "Command.h"
#include "../utils/String.h"

class AddToGroupCommand: public Command {
    unsigned int chatId;
    String username;

    public:
        AddToGroupCommand(unsigned int chatId, const String& username);

        void execute(System& system) const override;
};