#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <QString>
#include <QDateTime>
#include <QDataStream>

struct User;
struct ChatMessage;
struct RequestChatLogMessage;
struct ChatLogMessage;
struct RequestFriendListMessage;
struct FriendListMessage;
struct RequestFriendMessage;
struct RequestSignUpMessage;
struct SignUpCheckMessage;
struct RequestLoginMessage;
struct LoginCheckMessage;
struct ErrorMessage;

class Message
{
public:
    enum Type {
        CHAT_MESSAGE, 				 ///<聊天消息
        REQUEST_CHATLOG_MESSAGE,     ///<请求聊天记录消息
        CHATLOG_MESSAGE,             ///<返回聊天记录消息
        REQUEST_FRIENDLIST_MESSAGE,  ///<请求好友列表消息
        FRIENDLIST_MESSAGE,          ///<返回好友列表消息
        REQUEST_FRIEND_MESSAGE,      ///<请求添加好友、同意和拒接
        REQUEST_LOGIN_MESSAGE,       ///<请求登录消息
        LOGIN_CHECK_MESSAGE,         ///<登录成功消息
        REQUEST_SIGNUP_MESSAGE,      ///<请求注册消息
        SIGNUP_CHECK_MESSAGE,        ///<注册成功消息
        ERROR_MESSAGE
    };

    enum RequestStates {
        SUCCESS,
        FAIL
    };

public:

    static Type getType(QDataStream &dataSrc);
    static ChatMessage toChatMessage(QDataStream &dataSrc);
    static RequestChatLogMessage toRequestChatLogMessage(QDataStream &dataSrc);
    static ChatLogMessage toChatLogMessage(QDataStream &dataSrc);
    static RequestFriendListMessage toRequestFriendListMessage(QDataStream &dataSrc);
    static FriendListMessage toFriendListMessage(QDataStream &dataSrc);
    static RequestFriendMessage toRequestFriendMessage(QDataStream &dataSrc);
    static RequestSignUpMessage toRequestSignUpMessage(QDataStream &dataSrc);
    static SignUpCheckMessage toSignUpCheckMessage(QDataStream &dataSrc);
    static RequestLoginMessage toRequestLoginMessage(QDataStream &dataSrc);
    static LoginCheckMessage toLoginCheckMessage(QDataStream &dataSrc);
    static ErrorMessage toErrorMessage(QDataStream &dataSrc);

    static QByteArray FromChatMessage(const ChatMessage &msg);
    static QByteArray FromRequestChatLogMessage(const RequestChatLogMessage &msg);
    static QByteArray FromChatLogMessage(const ChatLogMessage &msg);
    static QByteArray FromRequestFriendListMessage(const RequestFriendListMessage &msg);
    static QByteArray FromFriendListMessage(const FriendListMessage &msg);
    static QByteArray FromRequestFriendMessage(const RequestFriendMessage &msg);
    static QByteArray FromRequestSignUpMessage(const RequestSignUpMessage &msg);
    static QByteArray FromSignUpCheckMessage(const SignUpCheckMessage &msg);
    static QByteArray FromRequestLoginMessage(const RequestLoginMessage &msg);
    static QByteArray FromLoginCheckMessage(const LoginCheckMessage &msg);
    static QByteArray FromErrorMessage(const ErrorMessage &msg);


}

#endif // MESSAGE_H
