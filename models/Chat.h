#pragma once

#include "../utils/String.h"
#include "ChatMessage.h"

template<typename T>
class Vector;

enum class ChatType {
    INDIVIDUAL,
    GROUP,
    DEFAULT = INDIVIDUAL
};

class Chat {
    unsigned int id;
    String name;
    ChatType type;

    Vector<ChatMessage>* messages;
    bool areMessagesLoaded = false;

    void freeDynamic();

    void copyDynamic(const Chat& other);

    public:
        Chat();

        Chat(unsigned int id, const String& name, ChatType type);

        Chat(unsigned int id, const String& name, const char* type);

        Chat(const Chat& other);

        Chat& operator=(const Chat& other);

        ~Chat();

        unsigned int getId() const;

        ChatType getChatType() const;

        const Vector<ChatMessage>* getMessages() const;

        Vector<ChatMessage>* getMessages();

        bool getAreMessagesLoaded() const;

        void loadMessages();

        friend std::ostream& operator<<(std::ostream& os, const Chat& chat);

        friend std::istream& operator>>(std::istream& is, Chat& chat);
};