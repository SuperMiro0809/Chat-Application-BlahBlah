#pragma once

#include "Database.h"

#include "../models/Chat.h"

class ChatsDatabase: public Database {
    void writeChatToTextFile(unsigned int id, const String& name, const char* type) const;

    void writeChatToBinaryFile(unsigned int id, const String& name, const char* type) const;

    public:
        ChatsDatabase(const char* dbName);

        unsigned int addNewChat(const String& name, const char* type) const;

        const Chat* findIndividualChat(unsigned int userOne, unsigned int userTwo);
};