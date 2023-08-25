#include "message.h"



Message::Type Message::getType(QDataStream &dataSrc) {
    qint32 ret;
    dataSrc >> ret;
    return (Message::Type)ret;
}

ChatMessage Message::toChatMessage(QDataStream &dataSrc) {
    ChatMessage ret;
    dataSrc >> ret.sendId >> ret.receiveId >> ret.msg >> ret.dateTime;
    return ret;
}

RequestChatLogMessage Message::toRequestChatLogMessage(QDataStream &dataSrc) {
    RequestChatLogMessage ret;
    dataSrc >> ret.requestId >> ret.friendId;
    return ret;
}

ChatLogMessage Message::toChatLogMessage(QDataStream &dataSrc) {
    ChatLogMessage ret;
    qint32 sz;
    dataSrc >> ret.requestId >> ret.friendId >> sz;
    while(sz--) {
        ChatMessage msg;
        dataSrc >> msg.sendId >> msg.receiveId >> msg.msg >> msg.dateTime;
        ret.messageList.append(msg);
    }
    return ret;
}

RequestFriendListMessage Message::toRequestFriendListMessage(QDataStream &dataSrc) {
    RequestFriendListMessage ret;
    dataSrc >> ret.requestId;
    return ret;
}

FriendListMessage Message::toFriendListMessage(QDataStream &dataSrc) {
    FriendListMessage ret;
    qint32 sz;
    dataSrc >> ret.requestId >> sz;
    while(sz--) {
        User user;
        dataSrc >> user.id >> user.username;
        ret.friendList.append(user);
    }
    return ret;
}

RequestFriendMessage Message::toRequestFriendMessage(QDataStream &dataSrc) {
    RequestFriendMessage ret;
    dataSrc >> ret.requestId >> ret.friendId;
    qint32 tmp;
    dataSrc >> tmp;
    ret.state = (Message::RequestStates)tmp;
    return ret;
}

RequestSignUpMessage Message::toRequestSignUpMessage(QDataStream &dataSrc) {
    RequestSignUpMessage ret;
    dataSrc >> ret.username >> ret.passward;
    return ret;
}

SignUpCheckMessage Message::toSignUpCheckMessage(QDataStream &dataSrc) {
    SignUpCheckMessage ret;
    qint32 tmp;
    dataSrc >> tmp >> ret.userId;
    ret.state = (Message::RequestStates)tmp;
    return ret;
}

RequestLoginMessage Message::toRequestLoginMessage(QDataStream &dataSrc) {
    RequestLoginMessage ret;
    dataSrc >> ret.id >> ret.passward;
    return ret;
}

LoginCheckMessage Message::toLoginCheckMessage(QDataStream &dataSrc) {
   LoginCheckMessage ret;
   qint32 tmp;
   dataSrc >> tmp >> ret.hint >> ret.username;
   ret.state = (Message::RequestStates)tmp;
   return ret;
}

ErrorMessage Message::toErrorMessage(QDataStream &dataSrc) {
    ErrorMessage ret;
    dataSrc >> ret.errorMsg;
    return ret;
}

QByteArray Message::FromChatMessage(const ChatMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << (qint32)CHAT_MESSAGE;
    in << msg.sendId << msg.receiveId << msg.msg << msg.dateTime;
    return ret;
}

QByteArray Message::FromRequestChatLogMessage(const RequestChatLogMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << (qint32)REQUEST_CHATLOG_MESSAGE;
    in << msg.requestId << msg.friendId;
    return ret;
}

QByteArray Message::FromChatLogMessage(const ChatLogMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << (qint32)CHATLOG_MESSAGE;
    qint32 sz = msg.messageList.length();
    in << msg.friendId << msg.requestId << sz;
    for(int i = 0; i < sz; i++) {
        const ChatMessage &chatMsg = msg.messageList[i];
        in << chatMsg.sendId << chatMsg.receiveId << chatMsg.msg << chatMsg.dateTime;
    }
    return ret;
}

QByteArray Message::FromRequestFriendListMessage(const RequestFriendListMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << (qint32)REQUEST_FRIENDLIST_MESSAGE;
    in << msg.requestId;
    return ret;
}

QByteArray Message::FromFriendListMessage(const FriendListMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << (qint32)FRIENDLIST_MESSAGE;
    qint32 sz = msg.friendList.length();
    in << msg.requestId << sz;
    for(int i = 0; i < sz; i++) {
        const User &friendUser = msg.friendList[i];
        in << friendUser.id << friendUser.username;
    }
    return ret;
}

QByteArray Message::FromRequestFriendMessage(const RequestFriendMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << (qint32)REQUEST_FRIEND_MESSAGE;
    in << msg.requestId << msg.friendId << (qint32)msg.state;
    return ret;
}

QByteArray Message::FromRequestSignUpMessage(const RequestSignUpMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << (qint32)REQUEST_SIGNUP_MESSAGE;
    in << msg.username << msg.passward;
    return ret;
}

QByteArray Message::FromSignUpCheckMessage(const SignUpCheckMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << (qint32)SIGNUP_CHECK_MESSAGE;
    in << (qint32)msg.state << msg.userId;
    return ret;
}

QByteArray Message::FromRequestLoginMessage(const RequestLoginMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << (qint32)REQUEST_LOGIN_MESSAGE;
    in << msg.id << msg.passward;
    return ret;
}

QByteArray Message::FromLoginCheckMessage(const LoginCheckMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << (qint32)LOGIN_CHECK_MESSAGE;
    in << (qint32)msg.state << msg.hint << msg.username;
    return ret;
}

QByteArray Message::FromErrorMessage(const ErrorMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << (qint32)ERROR_MESSAGE;
    in << msg.errorMsg;
    return ret;
}
