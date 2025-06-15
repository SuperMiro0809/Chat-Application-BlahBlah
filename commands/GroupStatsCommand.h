#pragma once

#include "Command.h"

class GroupStatsCommand: public Command {
    unsigned int chatId;

    public:
        GroupStatsCommand(unsigned int chatId);

        void execute(System& system) const override;
};