#pragma once

#include "Command.h"

class LogoutCommand: public Command {
    public:
        LogoutCommand() = default;

        void execute(System& system) override;
};