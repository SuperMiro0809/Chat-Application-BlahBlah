#pragma once

#include "../utils/String.h"
#include "../utils/Vector.hpp"

enum class ChatType {
    INDIVIDUAL,
    GROUP,
    DEFAULT = INDIVIDUAL
};

class Chat {
    unsigned int id;
    String name;
    ChatType type;

    Vector<ChatMessage> messages;
    bool areMessagesLoaded = false;

    public:
        Chat();

        Chat(unsigned int id, const String& name, ChatType type);

        Chat(unsigned int id, const String& name, const char* type);

        unsigned int getId() const;

        ChatType getChatType() const;

        bool getAreMessagesLoaded() const;

        void loadCriteriaChats();

        friend std::ostream& operator<<(std::ostream& os, const Chat& chat);

        friend std::istream& operator>>(std::istream& is, Chat& chat);
};