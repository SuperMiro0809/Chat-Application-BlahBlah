#include "Chat.h"

#include <sstream>
#include <stdexcept>

static const char* toString(ChatType type) {
    switch (type) {
        case ChatType::INDIVIDUAL: return "Individual";
        case ChatType::GROUP:      return "Group";
        default:                    return "Unknown";
    }
}

static ChatType parseChatType(const String& str) {
    if (str == "Individual") return ChatType::INDIVIDUAL;
    if (str == "Group") return ChatType::GROUP;

    return ChatType::DEFAULT;
}

static ChatType parseChatType(const char* str) {
    if (std::strcmp(str, "Individual") == 0) return ChatType::INDIVIDUAL;
    if (std::strcmp(str, "Group") == 0) return ChatType::GROUP;

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
    os << chat.id << '|' << chat.name << '|' << toString(chat.type);
    return os;
}

std::istream& operator>>(std::istream& is, Chat& chat) {
    String line;
    if (!getline(is, line)) {
        return is;
    }

    std::stringstream ss(line.getElements());
    String idStr, nameStr, typeStr;

    getline(ss, idStr, '|');
    getline(ss, nameStr, '|');
    getline(ss, typeStr);

    chat.id = std::atoi(idStr.getElements());
    chat.name = nameStr;
    chat.type = parseChatType(typeStr);

    return is;
}