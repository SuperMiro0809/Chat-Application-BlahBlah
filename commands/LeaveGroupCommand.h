#pragma once

#include "Command.h"

class LeaveGroupCommand: public Command {
    unsigned int chatId;

    public:
        LeaveGroupCommand(unsigned int chatId);

        void execute(System& system) const override;
};