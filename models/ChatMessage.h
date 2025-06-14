#pragma once

#include <ctime>
#include "../utils/String.h"

class ChatMessage {
    unsigned int id;
    unsigned int chatId;
    unsigned int senderId;
    String message;
    std::time_t sentAt;

    public:
        ChatMessage() = default;

        ChatMessage(
            unsigned int id,
            unsigned int chatId,
            unsigned int senderId,
            const String& message,
            std::time_t sentAt
        );

        unsigned int getChatId() const;

        void print() const;

        friend std::ostream& operator<<(std::ostream& os, const ChatMessage& msg);

        friend std::istream& operator>>(std::istream& is, ChatMessage& msg);
};