#pragma once

#include "../utils/String.h"
#include "Command.h"

class CommandFactory {
    public:
        static Command* create(const String& input);
};