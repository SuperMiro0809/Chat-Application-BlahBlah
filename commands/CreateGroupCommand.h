#pragma once

#include "Command.h"
#include "../utils/String.h"
#include "../utils/Vector.hpp"

class CreateGroupCommand: public Command {
    String groupName;
    Vector<String> participants;

    public:
        CreateGroupCommand(const String& groupName, const Vector<String>& participants);

        void execute(System& system) const override;
};