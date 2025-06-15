#pragma once

#include "Command.h"

class ViewChatsCommand: public Command {
    public:
        ViewChatsCommand() = default;

        void execute(System& system) const override;
};