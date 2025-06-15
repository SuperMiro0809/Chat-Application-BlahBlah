#include "ChatMessagesDatabase.h"

#include <fstream>
#include <stdexcept>
#include "../models/ChatMessage.h"
#include "../utils/Vector.hpp"

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

ChatMessage ChatMessagesDatabase::addNewMessage(unsigned int chatId, unsigned int senderId, const String& message) const {
    unsigned int nextId = autoIncrement();
    std::time_t now = std::time(nullptr);

    writeMessageToTextFile(nextId, chatId, senderId, message, now);
    writeMessageToBinaryFile(nextId, chatId, senderId, message, now);

    return ChatMessage(nextId, chatId, senderId, message, now);
}

void ChatMessagesDatabase::removeByUser(unsigned int userId) const {
    Vector<ChatMessage> messages;
    messages.loadFromFile(dbName.getElements());

    std::ofstream textFile((dbName + ".txt").getElements());
    std::ofstream binaryFile((dbName + ".dat").getElements(), std::ios::binary);

    if (!textFile.is_open() || !binaryFile.is_open()) {
        throw std::runtime_error("Error: could not open messages database for rewrite");
    }

    for (size_t i = 0; i < messages.getSize(); ++i) {
        const ChatMessage& m = messages[i];

        if (m.getSenderId() == userId) {
            continue; // Skip all messages sent by this user
        }

        // Write to text
        textFile << m << '\n';

        // Write to binary
        unsigned int id = m.getId();
        unsigned int chatId = m.getChatId();
        unsigned int senderId = m.getSenderId();

        const char* msg = m.getMessage().getElements();
        size_t msgLen = std::strlen(msg);
        std::time_t sentAt = m.getSentAt();

        binaryFile.write((const char*)&id, sizeof(id));
        binaryFile.write((const char*)&chatId, sizeof(chatId));
        binaryFile.write((const char*)&senderId, sizeof(senderId));
        binaryFile.write((const char*)&msgLen, sizeof(msgLen));
        binaryFile.write(msg, msgLen);
        binaryFile.write((const char*)&sentAt, sizeof(sentAt));
    }

    textFile.close();
    binaryFile.close();
}