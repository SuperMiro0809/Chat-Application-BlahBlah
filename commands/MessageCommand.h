#pragma once

#include "Command.h"
#include "../utils/String.h"

class MessageCommand: public Command {
    String username;
    String message;

    public:
        MessageCommand(const String& username, const String& message);

        void execute(System& system) const override;
};