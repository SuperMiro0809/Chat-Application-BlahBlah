#pragma once

#include <iostream>
#include "../models/ChatParticipant.h"

class ChatParticipantsVector {
    ChatParticipant* participants;
    size_t size;
    size_t capacity;

    void freeDynamic();

    void copyDynamic(const ChatParticipantsVector& other);

    void resize(size_t newCapacity);

    public:
        ChatParticipantsVector();

        ChatParticipantsVector(const ChatParticipantsVector& other);

        ChatParticipantsVector& operator=(const ChatParticipantsVector& other);

        ~ChatParticipantsVector();

        void addParticipant(const ChatParticipant& participant);

        size_t getSize() const;

        void loadFromFile(const char* filename, unsigned int chatId);

        const ChatParticipant& operator[](size_t index) const;

        friend std::ostream& operator<<(std::ostream& os, const ChatParticipantsVector& vector);
};