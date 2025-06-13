#include "ChatParticipantsVector.h"

#include <fstream>
#include "../core/SystemSettings.h"

void ChatParticipantsVector::freeDynamic() {
    delete[] participants;
}

void ChatParticipantsVector::copyDynamic(const ChatParticipantsVector& other) {
    ChatParticipant* newParticipants = new ChatParticipant[other.capacity];

    for (size_t i = 0; i < other.size; i++) {
        newParticipants[i] = other.participants[i];
    }

    delete[] participants;
    participants = newParticipants;
}

void ChatParticipantsVector::resize(size_t newCapacity) {
    ChatParticipant* temp = new ChatParticipant[newCapacity];

    for (size_t i = 0; i < size; i++) {
        temp[i] = participants[i];
    }

    delete[] participants;
    participants = temp;
}

ChatParticipantsVector::ChatParticipantsVector() {
    size = 0;
    capacity = 8;
    participants = new ChatParticipant[capacity];
}

ChatParticipantsVector::ChatParticipantsVector(const ChatParticipantsVector& other) {
    size = other.size;
    capacity = other.capacity;
    copyDynamic(other);
}

ChatParticipantsVector& ChatParticipantsVector::operator=(const ChatParticipantsVector& other) {
    if (this != &other) {
        freeDynamic();
        size = other.size;
        capacity = other.capacity;
        copyDynamic(other);
    }

    return *this;
}

ChatParticipantsVector::~ChatParticipantsVector() {
    freeDynamic();
}

void ChatParticipantsVector::addParticipant(const ChatParticipant& participant) {
    if (size >= capacity) {
        resize(capacity * 2);
    }

    participants[size++] = participant;
}

size_t ChatParticipantsVector::getSize() const {
    return size;
}

void ChatParticipantsVector::loadFromFile(const char* filename, unsigned int chatId) {
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
                addParticipant(participant);
            }
        }
    } else {
        DBFile.open(fileName.getElements(), std::ios::binary);

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        while (true) {
            ChatParticipant participant;
            DBFile.read((char*)(&participant), sizeof(ChatParticipant));
            if (DBFile.eof()) break;
            if (!DBFile) break;

            if (participant.getChatId() == chatId) {
                addParticipant(participant);
            }
        }
    }
}

const ChatParticipant& ChatParticipantsVector::operator[](size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Invalid participant index!");
    }
    return participants[index];
}