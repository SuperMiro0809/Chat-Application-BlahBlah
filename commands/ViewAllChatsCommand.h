#pragma once

#include "Command.h"

class ViewAllChatsCommand: public Command {
    public:
        ViewAllChatsCommand() = default;

        void execute(System& system) const override;

        bool isAllowed(System& system) const override;
};