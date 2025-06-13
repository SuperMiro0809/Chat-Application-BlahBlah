#include "ChatParticipantsDatabase.h"

#include <fstream>
#include "../models/ChatParticipant.h"

void ChatParticipantsDatabase::writeParticipantToTextFile(unsigned int id, unsigned int chatId, unsigned int userId, const char* type) const {
    ChatParticipant participant(id, chatId, userId, type);

    std::ofstream ofs((dbName + ".txt").getElements(), std::ios::app);

    if (!ofs.is_open()) {
        throw std::runtime_error("Error: could not open database file");
    }

    ofs << participant << '\n';
    ofs.close();
}

void ChatParticipantsDatabase::writeParticipantToBinaryFile(unsigned int id, unsigned int chatId, unsigned int userId, const char* type) const {
    std::ofstream ofs((dbName + ".dat").getElements(), std::ios::binary | std::ios::app);

    if (!ofs.is_open()) {
        throw std::runtime_error("Error: could not open database file");
    }

    ofs.write((const char*)&id, sizeof(id));
    ofs.write((const char*)&chatId, sizeof(chatId));
    ofs.write((const char*)&userId, sizeof(userId));

    unsigned int len = std::strlen(type);
    ofs.write((const char*)&len, sizeof(len));
    ofs.write(type, len);

    ofs.close();
}

ChatParticipantsDatabase::ChatParticipantsDatabase(const char* dbName): Database(dbName) {}

void ChatParticipantsDatabase::addNewParticipant(unsigned int chatId, unsigned int userId, const char* type) const {
    unsigned int nextId = autoIncrement();
    writeParticipantToTextFile(nextId, chatId, userId, type);
    writeParticipantToBinaryFile(nextId, chatId, userId, type);
}