#pragma once

#include "Command.h"
#include "../utils/String.h"

class KickFromGroupCommand: public Command {
    unsigned int chatId;
    String username;

    public:
        KickFromGroupCommand(unsigned int chatId, const String& username);

        void execute(System& system) const override;
};