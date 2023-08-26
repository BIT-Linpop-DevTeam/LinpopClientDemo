#ifndef MESSAGE_H
#define MESSAGE_H

/*  将包装的二进制数据解码
    QByteArray dataSrc; //收到的二进制数据
    QDataStream dataStream(&dataSrc, QIODevice::ReadOnly);
    Message::Type type = Message::getType(dataStream);

    switch (type) {
    case Message::CHAT_MESSAGE:
         = Message::toChatMessage(dataStream);

        break;
    case Message::REQUEST_FRIENDLIST_MESSAGE:
        break;
    case Message::FRIENDLIST_MESSAGE:
        break;
    case Message::REQUEST_CHATLOG_MESSAGE:
        break;
    case Message::CHATLOG_MESSAGE:
        break;
    case Message::REQUEST_FRIEND_MESSAGE:
        break;
    case Message::REQUEST_LOGIN_MESSAGE:
        break;
    case Message::LOGIN_CHECK_MESSAGE:
        break;
    case Message::REQUEST_SIGNUP_MESSAGE:
        break;
    case Message::SIGNUP_CHECK_MESSAGE:
        break;
    case Message::ERROR_MESSAGE:
        break;
    }
  */

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
};

struct ChatMessage {
    QString sendId, receiveId;
    QString msg;
    QDateTime dateTime;
    ChatMessage(const QString &sendId, const QString &receiveId, const QString &msg, const QDateTime &dateTime)
        :sendId(sendId), receiveId(receiveId), msg(msg), dateTime(dateTime) {}
    ChatMessage() {}
};

struct RequestChatLogMessage {
    QString requestId, friendId;
    RequestChatLogMessage(const QString &requestId, const QString &friendId)
        :requestId(requestId), friendId(friendId) {}
    RequestChatLogMessage() {}
};

struct ChatLogMessage {
    QString requestId, friendId;
    QList<ChatMessage> messageList;
    ChatLogMessage(const QString &requestId, const QString &friendId, const QList<ChatMessage> &list)
        :requestId(requestId), friendId(friendId)
    {
        for(int i = 0; i < list.length(); i++) {
            messageList.append(list.at(i));
        }
    }
    ChatLogMessage() {}
};

struct RequestFriendListMessage {
    QString requestId;
    RequestFriendListMessage(const QString &requestId)
        :requestId(requestId) {}
    RequestFriendListMessage() {}
};

struct User {
    QString id, username;
    User(const QString &id, const QString &username)
        :id(id), username(username) {}
    User() {}
};

struct FriendListMessage {
    QString requestId;
    QList<User> friendList;
    FriendListMessage(const QString &requestId, const QList<User> &list)
        :requestId(requestId)
    {
        for(int i = 0; i < list.length(); i++) {
            friendList.append(list.at(i));
        }
    }
    FriendListMessage() {}
};

struct RequestFriendMessage {
    QString requestId, friendId;
    Message::RequestStates state;
    RequestFriendMessage(const QString &requestId, const QString &friendId, const Message::RequestStates &state)
        :requestId(requestId), friendId(friendId), state(state)
    {}
    RequestFriendMessage() {}
};

struct RequestSignUpMessage {
    QString username, passward;
    RequestSignUpMessage(const QString &username, const QString &passward)
        :username(username), passward(passward)
    {}
    RequestSignUpMessage() {}
};

struct SignUpCheckMessage {
    Message::RequestStates state;
    QString userId;
    SignUpCheckMessage(const Message::RequestStates &state, const QString &userId)
        :state(state), userId(userId)
    {}
    SignUpCheckMessage() {}
};

struct RequestLoginMessage {
    QString id, password;
    RequestLoginMessage(const QString &id, const QString &password)
        :id(id), password(password)
    {}
    RequestLoginMessage() {}
};

struct LoginCheckMessage {
    Message::RequestStates state;
    QString hint, username;
    LoginCheckMessage(const Message::RequestStates &state, const QString &hint, const QString &username)
        :state(state), hint(hint), username(username)
    {}
    LoginCheckMessage() {}
};

struct ErrorMessage {
    QString errorMsg;
    ErrorMessage(const QString &errorMsg)
        :errorMsg(errorMsg)
    {}
    ErrorMessage() {}
};


#endif // MESSAGE_H
