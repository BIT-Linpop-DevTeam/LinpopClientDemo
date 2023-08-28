#include "message.h"



Message::Type Message::getType(QDataStream &dataSrc) {
    qint32 ret;
    dataSrc >> ret;
    return (Message::Type)ret;
}

ChatMessage Message::toChatMessage(QDataStream &dataSrc) {
    ChatMessage ret;
    dataSrc >> ret;
    return ret;
}

RequestChatLogMessage Message::toRequestChatLogMessage(QDataStream &dataSrc) {
    RequestChatLogMessage ret;
    dataSrc >> ret;
    return ret;
}

ChatLogMessage Message::toChatLogMessage(QDataStream &dataSrc) {
    ChatLogMessage ret;
    dataSrc >> ret;
    return ret;
}

RequestFriendListMessage Message::toRequestFriendListMessage(QDataStream &dataSrc) {
    RequestFriendListMessage ret;
    dataSrc >> ret;
    return ret;
}

FriendListMessage Message::toFriendListMessage(QDataStream &dataSrc) {
    FriendListMessage ret;
    dataSrc >> ret;
    return ret;
}

RequestFriendMessage Message::toRequestFriendMessage(QDataStream &dataSrc) {
    RequestFriendMessage ret;
    dataSrc >> ret;
    return ret;
}

RequestSignUpMessage Message::toRequestSignUpMessage(QDataStream &dataSrc) {
    RequestSignUpMessage ret;
    dataSrc >> ret;
    return ret;
}

SignUpCheckMessage Message::toSignUpCheckMessage(QDataStream &dataSrc) {
    SignUpCheckMessage ret;
    dataSrc >> ret;
    return ret;
}

RequestLoginMessage Message::toRequestLoginMessage(QDataStream &dataSrc) {
    RequestLoginMessage ret;
    dataSrc >> ret;
    return ret;
}

LoginCheckMessage Message::toLoginCheckMessage(QDataStream &dataSrc) {
    LoginCheckMessage ret;
    dataSrc >> ret;
    return ret;
}

ErrorMessage Message::toErrorMessage(QDataStream &dataSrc) {
    ErrorMessage ret;
    dataSrc >> ret;
    return ret;
}

RequestFriendRequestLogMessage Message::toRequestFriendRequestLogMessage(QDataStream &dataSrc){
    RequestFriendRequestLogMessage ret;
    dataSrc >> ret;
    return ret;
}

FriendRequestLogMessage Message::toFriendRequestLogMessage(QDataStream &dataSrc){
    FriendRequestLogMessage ret;
    dataSrc >> ret;
    return ret;
}

FileMessage Message::toFileMessage(QDataStream &dataSrc){
    FileMessage ret;
    dataSrc >> ret;
    return ret;
}

FriendStatusMessage Message::toFriendStatusMessage(QDataStream &dataSrc){
    FriendStatusMessage ret;
    dataSrc >> ret;
    return ret;
}


FindUserMessage Message::toFindUserMessage(QDataStream &dataSrc){
    FindUserMessage ret;
    dataSrc >> ret;
    return ret;
}

UserMessage Message::toUserMessage(QDataStream &dataSrc){
    UserMessage ret;
    dataSrc >> ret;
    return ret;
}




QByteArray Message::FromChatMessage(const ChatMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::CHAT_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromRequestChatLogMessage(const RequestChatLogMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::REQUEST_CHATLOG_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromChatLogMessage(const ChatLogMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::CHATLOG_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromRequestFriendListMessage(const RequestFriendListMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::REQUEST_FRIENDLIST_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromFriendListMessage(const FriendListMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::FRIENDLIST_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromRequestFriendMessage(const RequestFriendMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::REQUEST_FRIEND_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromRequestSignUpMessage(const RequestSignUpMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::REQUEST_SIGNUP_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromSignUpCheckMessage(const SignUpCheckMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::SIGNUP_CHECK_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromRequestLoginMessage(const RequestLoginMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::REQUEST_LOGIN_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromLoginCheckMessage(const LoginCheckMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::LOGIN_CHECK_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromErrorMessage(const ErrorMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::ERROR_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromRequestFriendRequestLogMessage
    (const RequestFriendRequestLogMessage &msg)
{
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::REQUEST_FRIEND_REQUESTLOG_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromFriendRequestLogMessage(const FriendRequestLogMessage &msg){
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::FRIEND_REQUESTLOG_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromFileMessage(const FileMessage &msg){
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::FILE_MESSAEG;
    in << msg;
    return ret;
}

QByteArray Message::FromFriendStatusMessage(const FriendStatusMessage &msg){
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::FRIEND_STATUS_MESSAGE;
    in << msg;
    return ret;
}


QByteArray Message::FromFindUserMessage(const FindUserMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::FIND_USER_MESSAGE;
    in << msg;
    return ret;
}

QByteArray Message::FromUserMessage(const UserMessage &msg) {
    QByteArray ret;
    QDataStream in(&ret, QIODevice::WriteOnly);
    in << Message::USER_MESSAGE;
    in << msg;
    return ret;
}
