#include "Chat.h"

#include <sstream>
#include <stdexcept>
#include "../core/Constants.h"
#include "../utils/Vector.hpp"

static const char* toString(ChatType type) {
    switch (type) {
        case ChatType::INDIVIDUAL: return CHAT_INDIVIDUAL;
        case ChatType::GROUP:      return CHAT_GROUP;
        default:                   return "Unknown";
    }
}

static ChatType parseChatType(const String& str) {
    if (str == CHAT_INDIVIDUAL) return ChatType::INDIVIDUAL;
    if (str == CHAT_GROUP) return ChatType::GROUP;

    return ChatType::DEFAULT;
}

static ChatType parseChatType(const char* str) {
    if (std::strcmp(str, CHAT_INDIVIDUAL) == 0) return ChatType::INDIVIDUAL;
    if (std::strcmp(str, CHAT_GROUP) == 0) return ChatType::GROUP;

    return ChatType::DEFAULT;
}

void Chat::freeDynamic() {
    delete messages;
    delete participants;
}

void Chat::copyDynamic(const Chat& other) {
    messages = new Vector(*other.messages);
    participants = new Vector(*other.participants);
}

Chat::Chat():
    id(-1),
    type(ChatType::DEFAULT),
    messages(new Vector<ChatMessage>()),
    participants(new Vector<ChatParticipant>()) {}

Chat::Chat(unsigned int id, const String& name, ChatType type):
    id(id),
    name(name),
    type(type),
    messages(new Vector<ChatMessage>()),
    participants(new Vector<ChatParticipant>()) {}

Chat::Chat(unsigned int id, const String& name, const char* type):
    id(id),
    name(name),
    messages(new Vector<ChatMessage>()),
    participants(new Vector<ChatParticipant>())
{
    this->type = parseChatType(type);
}

Chat::Chat(const Chat& other) {
    id = other.id;
    name = other.name;
    type = other.type;
    areMessagesLoaded = other.areMessagesLoaded;
    areParticipantsLoaded = other.areParticipantsLoaded;
    copyDynamic(other);
}

Chat::~Chat() {
    freeDynamic();
}

Chat &Chat::operator=(const Chat &other) {
    if (this != &other) {
        freeDynamic();
        id = other.id;
        name = other.name;
        type = other.type;
        areMessagesLoaded = other.areMessagesLoaded;
        areParticipantsLoaded = other.areParticipantsLoaded;
        copyDynamic(other);
    }
    return *this;
}

unsigned int Chat::getId() const {
    return id;
}

ChatType Chat::getChatType() const {
    return type;
}

const Vector<ChatMessage>* Chat::getMessages() const {
    return messages;
}

Vector<ChatMessage>* Chat::getMessages() {
    return messages;
}

bool Chat::getAreMessagesLoaded() const {
    return areMessagesLoaded;
}

void Chat::loadMessages() {
    messages->loadFromFileByCriteria(MESSAGES_DB_NAME, id);
    areMessagesLoaded = true;
}

const Vector<ChatParticipant>* Chat::getParticipants() const {
    return participants;
}

Vector<ChatParticipant>* Chat::getParticipants() {
    return participants;
}

bool Chat::getAreParticipantsLoaded() const {
    return areParticipantsLoaded;
}

void Chat::loadParticipants() {
    participants->loadFromFileByCriteria(PARTICIPANTS_DB_NAME, id);
    areParticipantsLoaded = true;
}

void Chat::print() {
    if (!areMessagesLoaded) {
        loadMessages();
    }

    std::cout << id << ": " << name << " (" << messages->getSize() << " messages)" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Chat& chat) {
    os << chat.id << FIELD_DELIMITER << chat.name << FIELD_DELIMITER << toString(chat.type);
    return os;
}

std::istream& operator>>(std::istream& is, Chat& chat) {
    String line;
    if (!getline(is, line)) {
        return is;
    }

    std::stringstream ss(line.getElements());
    String idStr, nameStr, typeStr;

    getline(ss, idStr, FIELD_DELIMITER);
    getline(ss, nameStr, FIELD_DELIMITER);
    getline(ss, typeStr);

    chat.id = std::atoi(idStr.getElements());
    chat.name = nameStr;
    chat.type = parseChatType(typeStr);

    return is;
}