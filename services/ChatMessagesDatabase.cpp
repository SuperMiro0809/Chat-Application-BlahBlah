#include "ChatMessagesDatabase.h"

#include <fstream>
#include <stdexcept>
#include "../models/ChatMessage.h"

void ChatMessagesDatabase::writeMessageToTextFile(unsigned int id, unsigned int chatId, unsigned int senderId, const String& message, std::time_t sentAt) const {
    ChatMessage msg(id, chatId, senderId, message, sentAt);

    std::ofstream ofs((dbName + ".txt").getElements(), std::ios::app);
    if (!ofs.is_open()) {
        throw std::runtime_error("Error: could not open database file");
    }

    ofs << msg << '\n';
    ofs.close();
}

void ChatMessagesDatabase::writeMessageToBinaryFile(unsigned int id, unsigned int chatId, unsigned int senderId, const String& message, std::time_t sentAt) const {
    std::ofstream ofs((dbName + ".dat").getElements(), std::ios::binary | std::ios::app);
    if (!ofs.is_open()) {
        throw std::runtime_error("Error: could not open database file");
    }

    ofs.write((const char*)(&id), sizeof(id));
    ofs.write((const char*)(&chatId), sizeof(chatId));
    ofs.write((const char*)(&senderId), sizeof(senderId));

    size_t messageLength = message.getSize();
    ofs.write((const char*)(&messageLength), sizeof(messageLength));
    ofs.write(message.getElements(), messageLength);

    ofs.write((const char*)(&sentAt), sizeof(sentAt));
    ofs.close();
}

ChatMessagesDatabase::ChatMessagesDatabase(const char* dbName): Database(dbName) {}

void ChatMessagesDatabase::addNewMessage(unsigned int chatId, unsigned int senderId, const String& message) const {
    unsigned int nextId = autoIncrement();
    std::time_t now = std::time(nullptr);

    writeMessageToTextFile(nextId, chatId, senderId, message, now);
    writeMessageToBinaryFile(nextId, chatId, senderId, message, now);
}