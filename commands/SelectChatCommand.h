#pragma once

#include "Command.h"

class SelectChatCommand: public Command {
    unsigned int chatId;

    public:
        SelectChatCommand(unsigned int chatId);

        void execute(System& system) const override;
};