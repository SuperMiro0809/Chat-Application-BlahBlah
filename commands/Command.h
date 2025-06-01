#pragma once

#include "../services/System.h"

class Command {
    public:
        virtual void execute(System& system) = 0;

        virtual ~Command() = default;
};