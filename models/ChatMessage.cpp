#include "ChatMessage.h"

#include <sstream>
#include "../core/Constants.h"

ChatMessage::ChatMessage(
    unsigned int id,
    unsigned int chatId,
    unsigned int senderId,
    const String& message,
    std::time_t sentAt
): id(id), chatId(chatId), senderId(senderId), message(message), sentAt(sentAt) {}

std::ostream& operator<<(std::ostream& os, const ChatMessage& msg) {
    os << msg.id << FIELD_DELIMITER
       << msg.chatId << FIELD_DELIMITER
       << msg.senderId << FIELD_DELIMITER
       << msg.message << FIELD_DELIMITER
       << msg.sentAt;
    return os;
}

std::istream& operator>>(std::istream& is, ChatMessage& msg) {
    String line;
    if (!getline(is, line)) {
        return is;
    }

    std::stringstream ss(line.getElements());
    String idStr, chatIdStr, senderIdStr, timestampStr;

    getline(ss, idStr, FIELD_DELIMITER);
    getline(ss, chatIdStr, FIELD_DELIMITER);
    getline(ss, senderIdStr, FIELD_DELIMITER);
    getline(ss, msg.message, FIELD_DELIMITER);
    getline(ss, timestampStr);

    msg.id = std::atoi(idStr.getElements());
    msg.chatId = std::atoi(chatIdStr.getElements());
    msg.senderId = std::atoi(senderIdStr.getElements());
    msg.sentAt = std::atoll(timestampStr.getElements());

    return is;
}