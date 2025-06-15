#include "ChatParticipantsDatabase.h"

#include <fstream>
#include "../models/ChatParticipant.h"
#include "../utils/Vector.hpp"

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

ChatParticipant ChatParticipantsDatabase::addNewParticipant(unsigned int chatId, unsigned int userId, const char* type) const {
    unsigned int nextId = autoIncrement();
    writeParticipantToTextFile(nextId, chatId, userId, type);
    writeParticipantToBinaryFile(nextId, chatId, userId, type);

    return ChatParticipant(nextId, chatId, userId, type);
}

void ChatParticipantsDatabase::removeParticipant(unsigned int chatId, unsigned int userId) {
    Vector<ChatParticipant> participants;
    participants.loadFromFile(dbName.getElements());

    std::ofstream textFile((dbName + ".txt").getElements());
    std::ofstream binaryFile((dbName + ".dat").getElements(), std::ios::binary);

    if (!textFile.is_open() || !binaryFile.is_open()) {
        throw std::runtime_error("Error: could not open database file for rewrite");
    }

    for (size_t i = 0; i < participants.getSize(); ++i) {
        const ChatParticipant& p = participants[i];

        // Skip the one we want to remove
        if (p.getChatId() == chatId && p.getUserId() == userId) {
            continue;
        }

        // Write to text
        textFile << p << '\n';

        // Write to binary
        unsigned int id = p.getId();
        unsigned int cid = p.getChatId();
        unsigned int uid = p.getUserId();
        const char* type =  p.getTypeStr();
        unsigned int len = std::strlen(type);

        binaryFile.write((const char*)&id, sizeof(id));
        binaryFile.write((const char*)&cid, sizeof(cid));
        binaryFile.write((const char*)&uid, sizeof(uid));
        binaryFile.write((const char*)&len, sizeof(len));
        binaryFile.write(type, len);
    }

    textFile.close();
    binaryFile.close();
}