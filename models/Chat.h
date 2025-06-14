#pragma once

#include "../utils/String.h"

enum class ChatType {
    INDIVIDUAL,
    GROUP,
    DEFAULT = INDIVIDUAL
};

class Chat {
    unsigned int id;
    String name;
    ChatType type;

    public:
        Chat();

        Chat(unsigned int id, const String& name, ChatType type);

        Chat(unsigned int id, const String& name, const char* type);

        unsigned int getId() const;

        ChatType getChatType() const;

        friend std::ostream& operator<<(std::ostream& os, const Chat& chat);

        friend std::istream& operator>>(std::istream& is, Chat& chat);
};