#include "ChatsDatabase.h"

#include <fstream>
#include <sstream>
#include "../models/Chat.h"
#include "../core/SystemSettings.h"
#include "../utils/ChatParticipantsVector.h"

void ChatsDatabase::writeChatToTextFile(unsigned int id, const String& name, const char* type) const {
    Chat chat(id, name, type);

    std::ofstream ofs((dbName + ".txt").getElements(), std::ios::app);

    if (!ofs.is_open()) {
        throw std::runtime_error("Error: could not open database file");
    }

    ofs << chat << '\n';
    ofs.close();
}

void ChatsDatabase::writeChatToBinaryFile(unsigned int id, const String& name, const char* type) const {
    std::ofstream ofs((dbName + ".dat").getElements(), std::ios::binary | std::ios::app);

    if (!ofs.is_open()) {
        throw std::runtime_error("Error: could not open database file");
    }

    ofs.write((const char*)&id, sizeof(id));

    unsigned int nameLen = name.getSize();
    ofs.write((const char*)(&nameLen), sizeof(nameLen));
    ofs.write(name.getElements(), nameLen);

    unsigned int len = std::strlen(type);
    ofs.write((const char*)&len, sizeof(len));
    ofs.write(type, len);

    ofs.close();
}

ChatsDatabase::ChatsDatabase(const char* dbName): Database(dbName) {}

unsigned int ChatsDatabase::addNewChat(const String& name, const char* type) const {
    unsigned int nextId = autoIncrement();
    writeChatToTextFile(nextId, name, type);
    writeChatToBinaryFile(nextId, name, type);

    return nextId;
}

const Chat* ChatsDatabase::findIndividualChat(unsigned int userOne, unsigned int userTwo) {
    SystemSettings systemSettings = SystemSettings::getInstance();
    FileMode mode = systemSettings.getFileMode();

    String fileName = systemSettings.getDbFileName(dbName);
    std::ifstream DBFile;

    if (mode == FileMode::TEXT) {
        DBFile.open(fileName.getElements());

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        Chat chat;
        while (DBFile >> chat) {
            if (chat.getChatType() == ChatType::INDIVIDUAL) {
                ChatParticipantsVector vector;
                vector.loadFromFile("../chat_participants", chat.getId());

                bool userOneFound = false;
                bool userTwoFound = false;
                size_t participantsLen = vector.getSize();

                if (participantsLen == 2) {
                    for (size_t i = 0; i < participantsLen; i++) {
                        if (vector[i].getUserId() == userOne) userOneFound = true;

                        if (vector[i].getUserId() == userTwo) userTwoFound = true;
                    }

                    if (userOneFound && userTwoFound) {
                        return new Chat(chat);
                    }
                }
            }
        }
    } else {
    }

    return nullptr;
}