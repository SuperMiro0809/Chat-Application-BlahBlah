#pragma once

#include "../services/System.h"

class Command {
    public:
        virtual void execute(System& system) const = 0;

        virtual bool isAllowed(System& system) const;

        virtual ~Command() = default;
};