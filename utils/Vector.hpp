#pragma once

#include <cstddef>
#include <stdexcept>
#include <iostream>
#include <fstream>

#include "../utils/String.h"
#include "../models/Chat.h"
#include "../models/ChatParticipant.h"
#include "../models/ChatMessage.h"
#include "../core/SystemSettings.h"
#include "../core/Constants.h"

template<typename T>
class Vector {
    T* data;
    size_t size;
    size_t capacity;

    void freeDynamic();

    void copyDynamic(const Vector& other);

    void resize(size_t newCapacity);

    public:
        Vector();

        Vector(const Vector& other);

        Vector& operator=(const Vector& other);

        ~Vector();

        void add(const T& element);

        size_t getSize() const;

        const T& operator[](size_t index) const;

        void loadFromFile(const char* filename);

        void loadFromFileByCriteria(const char* filename, unsigned int filterId);

        template<typename U>
        friend std::ostream& operator<<(std::ostream& os, const Vector<U>& vector);
};

template<typename T>
void Vector<T>::freeDynamic() {
    delete[] data;
}

template<typename T>
void Vector<T>::copyDynamic(const Vector& other) {
    data = new T[other.capacity];

    for (size_t i = 0; i < other.size; i++) {
        data[i] = other.data[i];
    }
}

template<typename T>
void Vector<T>::resize(size_t newCapacity) {
    capacity = newCapacity;
    T* temp = new T[capacity];

    for (size_t i = 0; i < size; i++) {
        temp[i] = data[i];
    }

    delete[] data;
    data = temp;
}

template<typename T>
Vector<T>::Vector() {
    size = 0;
    capacity = 8;
    data = new T[capacity];
}

template<typename T>
Vector<T>::Vector(const Vector& other) {
    size = other.size;
    capacity = other.capacity;
    copyDynamic(other);
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this != &other) {
        freeDynamic();
        size = other.size;
        capacity = other.capacity;
        copyDynamic(other);
    }
    return *this;
}

template<typename T>
Vector<T>::~Vector() {
    freeDynamic();
}

template<typename T>
void Vector<T>::add(const T& element) {
    if (size == capacity) {
        resize(capacity == 0 ? 2 : capacity * 2);
    }

    data[size++] = element;
}

template<typename T>
size_t Vector<T>::getSize() const {
    return size;
}

template<typename T>
const T& Vector<T>::operator[](size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }

    return data[index];
}

// ========== Specializations ==========

template<>
inline void Vector<ChatParticipant>::loadFromFile(const char* filename) {
    SystemSettings systemSettings = SystemSettings::getInstance();
    FileMode mode = systemSettings.getFileMode();

    String fileName = systemSettings.getDbFileName(String(filename));
    std::ifstream DBFile;

    if (mode == FileMode::TEXT) {
        DBFile.open(fileName.getElements());

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        ChatParticipant participant;
        while (DBFile >> participant) {
            add(participant);
        }
    } else {
        DBFile.open(fileName.getElements(), std::ios::binary);

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        while (!DBFile.eof()) {
            unsigned int id, chatId, userId, len;

            DBFile.read((char*)(&id), sizeof(id));
            DBFile.read((char*)(&chatId), sizeof(chatId));
            DBFile.read((char*)(&userId), sizeof(userId));

            DBFile.read((char*)(&len), sizeof(len));
            char* currType = new char[len + 1];
            DBFile.read(currType, len);
            currType[len] = '\0';

            ChatParticipant participant(id, chatId, userId, currType);
            add(participant);
            delete[] currType;
        }
    }

    DBFile.close();
}

template<>
inline void Vector<Chat>::loadFromFileByCriteria(const char* filename, unsigned int userId) {
    SystemSettings systemSettings = SystemSettings::getInstance();
    FileMode mode = systemSettings.getFileMode();

    String fileName = systemSettings.getDbFileName(String(filename));
    std::ifstream DBFile;

    Vector<ChatParticipant> participants;
    participants.loadFromFile(PARTICIPANTS_DB_NAME);

    if (mode == FileMode::TEXT) {
        DBFile.open(fileName.getElements());

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        Chat chat;
        while (DBFile >> chat) {
            for (size_t i = 0; i < participants.getSize(); i++) {
                if (participants[i].getChatId() == chat.getId() && participants[i].getUserId() == userId) {
                    add(chat);
                    break;
                }
            }
        }
    } else {
        DBFile.open(fileName.getElements(), std::ios::binary);

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        while (!DBFile.eof()) {
            unsigned int id, nameLen, typeLen;

            DBFile.read((char*)(&id), sizeof(id));

            DBFile.read((char*)(&nameLen), sizeof(nameLen));
            char* currName = new char[nameLen + 1];
            DBFile.read(currName, nameLen);
            currName[nameLen] = '\0';

            DBFile.read((char*)(&typeLen), sizeof(typeLen));
            char* currType = new char[typeLen + 1];
            DBFile.read(currType, typeLen);
            currType[typeLen] = '\0';

            Chat chat(id, String(currName), currType);

            delete[] currName;
            delete[] currType;

            for (size_t i = 0; i < participants.getSize(); ++i) {
                if (participants[i].getChatId() == id && participants[i].getUserId() == userId) {
                    add(chat);
                    break;
                }
            }
        }
    }

    DBFile.close();
}

template<>
inline void Vector<ChatParticipant>::loadFromFileByCriteria(const char* filename, unsigned int chatId) {
    SystemSettings systemSettings = SystemSettings::getInstance();
    FileMode mode = systemSettings.getFileMode();

    String fileName = systemSettings.getDbFileName(String(filename));
    std::ifstream DBFile;

    if (mode == FileMode::TEXT) {
        DBFile.open(fileName.getElements());

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        ChatParticipant participant;
        while (DBFile >> participant) {
            if (participant.getChatId() == chatId) {
                add(participant);
            }
        }
    } else {
        DBFile.open(fileName.getElements(), std::ios::binary);

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        while (true) {
            unsigned int id, currChatId, userId, len;

            DBFile.read((char*)(&id), sizeof(id));
            if (DBFile.eof()) break;

            DBFile.read((char*)(&currChatId), sizeof(currChatId));
            if (DBFile.eof()) break;

            DBFile.read((char*)(&userId), sizeof(userId));
            if (DBFile.eof()) break;

            DBFile.read((char*)(&len), sizeof(len));
            char* currType = new char[len + 1];
            DBFile.read(currType, len);
            currType[len] = '\0';

            if (currChatId == chatId) {
                ChatParticipant participant(id, chatId, userId, currType);
                delete[] currType;

                add(participant);
            }
        }
    }

    DBFile.close();
}

template<>
inline void Vector<ChatMessage>::loadFromFileByCriteria(const char* filename, unsigned int chatId) {
    SystemSettings systemSettings = SystemSettings::getInstance();
    FileMode mode = systemSettings.getFileMode();

    String fileName = systemSettings.getDbFileName(String(filename));
    std::ifstream DBFile;

    if (mode == FileMode::TEXT) {
        DBFile.open(fileName.getElements());

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        ChatMessage message;
        while (DBFile >> message) {
            if (message.getChatId() == chatId) {
                add(message);
            }
        }
    } else {
        DBFile.open(fileName.getElements(), std::ios::binary);

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        while (true) {
            unsigned int id, currChatId, senderId, messageLen;
            std::time_t sentAt;

            DBFile.read((char*)(&id), sizeof(id));
            if (DBFile.eof()) break;

            DBFile.read((char*)(&currChatId), sizeof(currChatId));
            if (DBFile.eof()) break;

            DBFile.read((char*)(&senderId), sizeof(senderId));
            if (DBFile.eof()) break;

            DBFile.read((char*)(&messageLen), sizeof(messageLen));
            if (DBFile.eof()) break;

            char* currMessage = new char[messageLen + 1];
            if (!DBFile.read(currMessage, messageLen)) {
                delete[] currMessage;
                break;;
            }
            currMessage[messageLen] = '\0';

            DBFile.read((char*)(&sentAt), sizeof(sentAt));
            if (DBFile.eof()) {
                delete[] currMessage;
                break;
            }

            if (currChatId == chatId) {
                ChatMessage message(id, chatId, senderId, String(currMessage), sentAt);
                delete[] currMessage;

                add(message);
            }
        }
    }

    DBFile.close();
}