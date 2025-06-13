#include "ChatParticipant.h"

#include <sstream>
#include "../utils/String.h"

static const char* toString(ParticipantTypes type) {
    switch (type) {
        case ParticipantTypes::MEMBER: return "Member";
        case ParticipantTypes::ADMIN:  return "Admin";
        default:                       return "Unknown";
    }
}

static ParticipantTypes parseParticipantType(const String& str) {
    if (str == "Member") return ParticipantTypes::MEMBER;
    if (str == "Admin") return ParticipantTypes::ADMIN;

    return ParticipantTypes::DEFAULT;
}

static ParticipantTypes parseParticipantType(const char* str) {
    if (std::strcmp(str, "Member") == 0) return ParticipantTypes::MEMBER;
    if (std::strcmp(str, "Admin") == 0) return ParticipantTypes::ADMIN;

    return ParticipantTypes::DEFAULT;
}

ChatParticipant::ChatParticipant(): id(-1), chatId(-1), userId(-1), type(ParticipantTypes::DEFAULT) {}

ChatParticipant::ChatParticipant(unsigned int id, unsigned int chatId, unsigned int userId, ParticipantTypes type):
    id(id),
    chatId(chatId),
    userId(userId),
    type(type) {}

ChatParticipant::ChatParticipant(unsigned int id, unsigned int chatId, unsigned int userId, const char* type):
    id(id),
    chatId(chatId),
    userId(userId)
{
    this->type = parseParticipantType(type);
}

unsigned int ChatParticipant::getChatId() const {
    return chatId;
}

unsigned int ChatParticipant::getUserId() const {
    return userId;
}

std::ostream& operator<<(std::ostream& os, const ChatParticipant& participant) {
    os << participant.id << '|'
       << participant.chatId << '|'
       << participant.userId << '|'
       << toString(participant.type);
    return os;
}

std::istream& operator>>(std::istream& is, ChatParticipant& participant) {
    String line;
    if (!getline(is, line))
        return is;

    std::stringstream ss(line.getElements());
    String idStr, chatIdStr, userIdStr, typeStr;

    getline(ss, idStr, '|');
    getline(ss, chatIdStr, '|');
    getline(ss, userIdStr, '|');
    getline(ss, typeStr);

    participant.id = std::atoi(idStr.getElements());
    participant.chatId = std::atoi(chatIdStr.getElements());
    participant.userId = std::atoi(userIdStr.getElements());
    participant.type = parseParticipantType(typeStr);

    return is;
}
