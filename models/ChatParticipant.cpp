#include "ChatParticipant.h"

#include <sstream>
#include "../utils/String.h"
#include "../core/Constants.h"

static const char* toString(ParticipantTypes type) {
    switch (type) {
        case ParticipantTypes::MEMBER: return PARTICIPANT_MEMBER;
        case ParticipantTypes::ADMIN:  return PARTICIPANT_ADMIN;
        default:                       return "Unknown";
    }
}

static ParticipantTypes parseParticipantType(const String& str) {
    if (str == PARTICIPANT_MEMBER) return ParticipantTypes::MEMBER;
    if (str == PARTICIPANT_ADMIN) return ParticipantTypes::ADMIN;

    return ParticipantTypes::DEFAULT;
}

static ParticipantTypes parseParticipantType(const char* str) {
    if (std::strcmp(str, PARTICIPANT_MEMBER) == 0) return ParticipantTypes::MEMBER;
    if (std::strcmp(str, PARTICIPANT_ADMIN) == 0) return ParticipantTypes::ADMIN;

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

unsigned int ChatParticipant::getId() const {
    return id;
}

unsigned int ChatParticipant::getChatId() const {
    return chatId;
}

unsigned int ChatParticipant::getUserId() const {
    return userId;
}

ParticipantTypes ChatParticipant::getType() const {
    return type;
}

const char* ChatParticipant::getTypeStr() const {
    return toString(type);
}

void ChatParticipant::setType(ParticipantTypes newType) {
    type = newType;
}

void ChatParticipant::setType(const char* newType) {
    type = parseParticipantType(newType);
}

std::ostream& operator<<(std::ostream& os, const ChatParticipant& participant) {
    os << participant.id << FIELD_DELIMITER
       << participant.chatId << FIELD_DELIMITER
       << participant.userId << FIELD_DELIMITER
       << toString(participant.type);
    return os;
}

std::istream& operator>>(std::istream& is, ChatParticipant& participant) {
    String line;
    if (!getline(is, line))
        return is;

    std::stringstream ss(line.getElements());
    String idStr, chatIdStr, userIdStr, typeStr;

    getline(ss, idStr, FIELD_DELIMITER);
    getline(ss, chatIdStr, FIELD_DELIMITER);
    getline(ss, userIdStr, FIELD_DELIMITER);
    getline(ss, typeStr);

    participant.id = std::atoi(idStr.getElements());
    participant.chatId = std::atoi(chatIdStr.getElements());
    participant.userId = std::atoi(userIdStr.getElements());
    participant.type = parseParticipantType(typeStr);

    return is;
}
