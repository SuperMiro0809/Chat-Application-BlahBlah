#pragma once

#include "Database.h"

#include <ctime>

class ChatMessagesDatabase: public Database {
    void writeMessageToTextFile(unsigned int id, unsigned int chatId, unsigned int senderId, const String& message, std::time_t sentAt) const;

    void writeMessageToBinaryFile(unsigned int id, unsigned int chatId, unsigned int senderId, const String& message, std::time_t sentAt) const;

    public:
        ChatMessagesDatabase(const char* dbName);

        void addNewMessage(unsigned int chatId, unsigned int senderId, const String& message) const;
};