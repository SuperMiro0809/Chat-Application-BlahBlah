#pragma once

#include "Database.h"
#include "../models/ChatParticipant.h"

class ChatParticipantsDatabase: public Database {
    void writeParticipantToTextFile(unsigned int id, unsigned int chatId, unsigned int userId, const char* type) const;

    void writeParticipantToBinaryFile(unsigned int id, unsigned int chatId, unsigned int userId, const char* type) const;

    public:
        ChatParticipantsDatabase(const char* dbName);

        ChatParticipant addNewParticipant(unsigned int chatId, unsigned int userId, const char* type) const;

        void removeParticipant(unsigned int chatId, unsigned int userId);
};