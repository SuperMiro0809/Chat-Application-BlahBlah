#include "Chat.h"

#include <sstream>
#include <stdexcept>
#include "../core/Constants.h"

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

Chat::Chat(): id(-1), type(ChatType::DEFAULT) {}

Chat::Chat(unsigned int id, const String& name, ChatType type): id(id), name(name), type(type) {}

Chat::Chat(unsigned int id, const String& name, const char* type): id(id), name(name) {
    this->type = parseChatType(type);
}

unsigned int Chat::getId() const {
    return id;
}

ChatType Chat::getChatType() const {
    return type;
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