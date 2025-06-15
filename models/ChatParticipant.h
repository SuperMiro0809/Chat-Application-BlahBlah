#pragma once

#include <iostream>

enum class ParticipantTypes {
    MEMBER,
    ADMIN,
    DEFAULT = MEMBER
};

class ChatParticipant {
    unsigned int id;
    unsigned int chatId;
    unsigned int userId;
    ParticipantTypes type;

    public:
        ChatParticipant();

        ChatParticipant(unsigned int id, unsigned int chatId, unsigned int userId, ParticipantTypes type);

        ChatParticipant(unsigned int id, unsigned int chatId, unsigned int userId, const char* type);

        unsigned int getChatId() const;

        unsigned int getUserId() const;

        ParticipantTypes getType() const;

        friend std::ostream& operator<<(std::ostream& os, const ChatParticipant& participant);

        friend std::istream& operator>>(std::istream& is, ChatParticipant& participant);
};