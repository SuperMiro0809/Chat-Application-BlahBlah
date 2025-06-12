#pragma once

#include "Command.h"
#include "../services/SystemSettings.h"

class FileModeCommand: public Command {
    public:
        FileModeCommand() = default;

        void execute(System& system) const override;

        bool isAllowed(System& system) const override;
};