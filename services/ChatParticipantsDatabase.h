#pragma once

#include "Database.h"

class ChatParticipantsDatabase: public Database {
    void writeParticipantToTextFile(unsigned int id, unsigned int chatId, unsigned int userId, const char* type) const;

    void writeParticipantToBinaryFile(unsigned int id, unsigned int chatId, unsigned int userId, const char* type) const;

    public:
        ChatParticipantsDatabase(const char* dbName);

        void addNewParticipant(unsigned int chatId, unsigned int userId, const char* type) const;
};