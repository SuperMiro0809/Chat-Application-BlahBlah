#include "ChatsDatabase.h"

#include <fstream>
#include <sstream>
#include "../models/Chat.h"
#include "../core/SystemSettings.h"
#include "../core/Constants.h"
#include "../utils/Vector.hpp"

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

const Chat* ChatsDatabase::findIndividualChat(unsigned int userOne, unsigned int userTwo) const {
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
                Vector<ChatParticipant> vector;
                vector.loadFromFileByCriteria(PARTICIPANTS_DB_NAME, chat.getId());

                bool userOneFound = false;
                bool userTwoFound = false;
                size_t participantsLen = vector.getSize();

                if (participantsLen == 2) {
                    for (size_t i = 0; i < participantsLen; i++) {
                        if (vector[i].getUserId() == userOne) userOneFound = true;

                        if (vector[i].getUserId() == userTwo) userTwoFound = true;
                    }

                    if (userOneFound && userTwoFound) {
                        DBFile.close();
                        return new Chat(chat);
                    }
                }
            }
        }
    } else {
        DBFile.open(fileName.getElements(), std::ios::binary);

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        while (true) {
            unsigned int id, nameLen, typeLen;

            DBFile.read((char*)(&id), sizeof(id));
            if (DBFile.eof()) break;

            DBFile.read((char*)(&nameLen), sizeof(nameLen));
            if (DBFile.eof()) break;

            char* nameBuf = new char[nameLen + 1];
            if (!DBFile.read(nameBuf, nameLen)) {
                break;
            }
            nameBuf[nameLen] = '\0';

            DBFile.read((char*)(&typeLen), sizeof(typeLen));
            if (DBFile.eof()) break;

            char* typeBuf = new char[typeLen + 1];
            if (!DBFile.read(typeBuf, typeLen)) {
                delete[] nameBuf;
                break;
            }
            typeBuf[typeLen] = '\0';

            Chat chat(id, String(nameBuf), typeBuf);
            delete[] nameBuf;
            delete[] typeBuf;

            if (chat.getChatType() == ChatType::INDIVIDUAL) {
                Vector<ChatParticipant> vector;
                vector.loadFromFileByCriteria(PARTICIPANTS_DB_NAME, chat.getId());

                bool userOneFound = false;
                bool userTwoFound = false;
                size_t participantsLen = vector.getSize();

                if (participantsLen == 2) {
                    for (size_t i = 0; i < participantsLen; i++) {
                        if (vector[i].getUserId() == userOne) userOneFound = true;
                        if (vector[i].getUserId() == userTwo) userTwoFound = true;
                    }

                    if (userOneFound && userTwoFound) {
                        DBFile.close();
                        return new Chat(chat);
                    }
                }
            }
        }
    }

    DBFile.close();
    return nullptr;
}

const Chat* ChatsDatabase::findById(unsigned int chatId) const {
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
            if (chat.getId() == chatId) {
                DBFile.close();
                return new Chat(chat);
            }
        }
    } else {
        DBFile.open(fileName.getElements(), std::ios::binary);

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        while (!DBFile.eof()) {
            unsigned int id;
            unsigned int nameLen;
            unsigned int typeLen;

            DBFile.read((char*)(&id), sizeof(id));
            DBFile.read((char*)(&nameLen), sizeof(nameLen));

            char* currName = new char[nameLen + 1];
            DBFile.read(currName, nameLen);

            DBFile.read((char*)(&typeLen), sizeof(typeLen));

            char* currType = new char[typeLen + 1];
            DBFile.read(currType, typeLen);

            if (id == chatId) {
                DBFile.close();
                return new Chat(id, String(currName), currType);
            }
        }
    }

    DBFile.close();
    return nullptr;
}