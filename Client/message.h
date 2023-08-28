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
#include <QObject>
struct User;

// 登录注册用的消息
struct RequestSignUpMessage;
struct SignUpCheckMessage;
struct RequestLoginMessage;
struct LoginCheckMessage;

// 登录初始化需要的消息
struct RequestChatLogMessage;
struct ChatLogMessage;
struct RequestFriendListMessage;
struct FriendListMessage;

// 请求好友请求列表的话, 看客户端想什么时候请求了, 想点击请求时再请求也可以, 想登录后请求也可以。
struct RequestFriendRequestLogMessage;
struct FriendRequestLogMessage;

// 用户实际发送的消息
struct ChatMessage;
struct RequestFriendMessage;
struct FriendStatusMessage;
struct FileMessage;

// 发送查找某用户的消息
struct FindUserMessage;
// 发送用户相关消息, 既作为查找用户消息的回复，也作为正常的用户设置头像和昵称时发送的消息
struct UserMessage;

// 错误消息
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
        REQUEST_FRIEND_MESSAGE,      ///<请求添加消息
        FRIEND_STATUS_MESSAGE,      ///<好友请求状态消息
        REQUEST_LOGIN_MESSAGE,       ///<请求登录消息
        LOGIN_CHECK_MESSAGE,         ///<登录成功消息
        REQUEST_SIGNUP_MESSAGE,      ///<请求注册消息
        SIGNUP_CHECK_MESSAGE,        ///<注册成功消息
        ERROR_MESSAGE,               ///<错误消息
        REQUEST_FRIEND_REQUESTLOG_MESSAGE, ///<请求好友请求列表消息
        FRIEND_REQUESTLOG_MESSAGE,   ///<好友请求列表消息
        FIND_USER_MESSAGE,           ///<用户查询消息
        FILE_MESSAEG,                ///<文件消息
        USER_MESSAGE,                ///<用户信息消息
    };

    enum RequestStates {
        SUCCESS,
        FAIL,
        UNTREATED,
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
    static RequestFriendRequestLogMessage toRequestFriendRequestLogMessage(QDataStream &dataSrc);
    static FriendRequestLogMessage toFriendRequestLogMessage(QDataStream &dataSrc);
    static FileMessage toFileMessage(QDataStream &dataSrc);
    static FriendStatusMessage toFriendStatusMessage(QDataStream &dataSrc);
    static FindUserMessage toFindUserMessage(QDataStream &dataSrc);
    static UserMessage toUserMessage(QDataStream &dataSrc);

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
    static QByteArray FromRequestFriendRequestLogMessage(const RequestFriendRequestLogMessage &msg);
    static QByteArray FromFriendRequestLogMessage(const FriendRequestLogMessage &msg);
    static QByteArray FromFileMessage(const FileMessage &msg);
    static QByteArray FromFriendStatusMessage(const FriendStatusMessage &msg);
    static QByteArray FromFindUserMessage(const FindUserMessage &msg);
    static QByteArray FromUserMessage(const UserMessage &msg);
};

struct ChatMessage {
    QString sendId, receiveId;
    QString msg;
    QDateTime dateTime;
    ChatMessage(const QString &sendId, const QString &receiveId, const QString &msg, const QDateTime &dateTime)
        :sendId(sendId), receiveId(receiveId), msg(msg), dateTime(dateTime) {}
    ChatMessage() {}
    friend QDataStream &operator <<(QDataStream &out, const ChatMessage &msg){
        out << msg.sendId << msg.receiveId << msg.msg << msg.dateTime;
        return out;
    }
    friend QDataStream &operator >>(QDataStream &in, ChatMessage &msg){
        in >> msg.sendId >> msg.receiveId >> msg.msg >> msg.dateTime;
        return in;
    }
};

struct RequestChatLogMessage {
    QString requestId, friendId;
    RequestChatLogMessage(const QString &requestId, const QString &friendId)
        :requestId(requestId), friendId(friendId) {}
    RequestChatLogMessage() {}
    friend QDataStream &operator <<(QDataStream &out, const RequestChatLogMessage &msg){
        out << msg.requestId << msg.friendId;
        return out;
    }
    friend QDataStream & operator >> (QDataStream &in, RequestChatLogMessage &msg){
        in >> msg.requestId >> msg.friendId;
        return in;
    }
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
    friend QDataStream &operator <<(QDataStream &out, const ChatLogMessage &msg){
        out << msg.requestId << msg.friendId << msg.messageList.length();
        for (auto &chtMsg:msg.messageList)
        {
            out << chtMsg;
        }
        return out;
    }
    friend QDataStream & operator >> (QDataStream &in, ChatLogMessage &msg){
        qint32 sz;
        in >> msg.requestId >> msg.friendId >> sz;
        for (int i = 0; i < sz; i++) {
            ChatMessage tmp;
            in >> tmp;
            msg.messageList.append(tmp);
        }
        return in;
    }
};

struct RequestFriendListMessage {
    QString requestId;
    RequestFriendListMessage(const QString &requestId)
        :requestId(requestId) {}
    RequestFriendListMessage() {}
    friend QDataStream &operator <<(QDataStream &out, const RequestFriendListMessage &msg){
        out << msg.requestId;
        return out;
    }
    friend QDataStream & operator >> (QDataStream &in, RequestFriendListMessage &msg){
        in >> msg.requestId;
        return in;
    }
};

struct User {
    QString id, username;
    qint32 avatar;
    User(const QString &id, const QString &username, const qint32 &avatar = 0)
        :id(id), username(username), avatar(avatar) {}
    User() {}
    friend QDataStream &operator<<(QDataStream &out, const User &user) {
        out << user.id << user.username << user.avatar;
        return out;
    }
    friend QDataStream &operator>>(QDataStream &in, User &user) {
        in >> user.id >> user.username >> user.avatar;
        return in;
    }
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
    friend QDataStream &operator<<(QDataStream &out, const FriendListMessage &msg) {
        out << msg.requestId;
        out << msg.friendList.size();
        for (int i = 0; i < msg.friendList.length(); i++) out << msg.friendList.at(i);
        return out;
    }
    friend QDataStream &operator>>(QDataStream &in, FriendListMessage &msg) {
        in >> msg.requestId;
        qint32 sz;
        in >> sz;
        for (int i = 0; i < sz; i++) {
            User tmp;
            in >> tmp;
            msg.friendList.append(tmp);
        }
        return in;
    }
};

struct RequestFriendMessage {
    QString requestId, friendId; Message::RequestStates states; QDateTime dateTime;
    RequestFriendMessage(const QString &requestId, const QString &friendId,
                         const Message::RequestStates &states,
                         const QDateTime &dateTime = QDateTime::currentDateTime())
        :requestId(requestId), friendId(friendId), states(states), dateTime(dateTime)
    {}
    RequestFriendMessage() {}
    friend QDataStream &operator <<(QDataStream &out, const RequestFriendMessage &msg){
        out << msg.requestId << msg.friendId << msg.states << msg.dateTime;
        return out;
    }
    friend QDataStream &operator >>(QDataStream &in, RequestFriendMessage &msg){
        in >> msg.requestId >> msg.friendId >> msg.states >> msg.dateTime;
        return in;
    }
};

struct FriendStatusMessage{
    User requestUser, friendUser; QDateTime dateTime;
    Message::RequestStates states;
    FriendStatusMessage(const User &requestUser, const User &friendUser, const QDateTime &dateTime,
                        const Message::RequestStates &states):
        requestUser(requestUser), friendUser(friendUser), dateTime(dateTime), states(states)
    {

    }
    FriendStatusMessage(){}
    friend QDataStream &operator <<(QDataStream &out, const FriendStatusMessage &msg){
        out << msg.requestUser << msg.friendUser << msg.dateTime << msg.states;
        return out;
    }
    friend QDataStream &operator >>(QDataStream &in, FriendStatusMessage &msg){
        in >> msg.requestUser >> msg.friendUser >> msg.dateTime >> msg.states;
        return in;
    }
};

struct RequestSignUpMessage {
    QString username, password;
    RequestSignUpMessage(const QString &username, const QString &password)
        :username(username), password(password)
    {}
    RequestSignUpMessage() {}
    friend QDataStream &operator <<(QDataStream &out, const RequestSignUpMessage &msg){
        out << msg.username << msg.password;
        return out;
    }
    friend QDataStream &operator >>(QDataStream &in, RequestSignUpMessage &msg){
        in >> msg.username >> msg.password;
        return in;
    }
};

struct SignUpCheckMessage {
    Message::RequestStates state;
    QString userId;
    SignUpCheckMessage(const Message::RequestStates &state, const QString &userId)
        :state(state), userId(userId)
    {}
    SignUpCheckMessage() {}
    friend QDataStream &operator <<(QDataStream &out, const SignUpCheckMessage &msg){
        out << msg.state << msg.userId;
        return out;
    }
    friend QDataStream &operator >>(QDataStream &in, SignUpCheckMessage &msg){
        in >> msg.state >> msg.userId;
        return in;
    }
};

struct RequestLoginMessage {
    QString id, password;
    RequestLoginMessage(const QString &id, const QString &password)
        :id(id), password(password)
    {}
    RequestLoginMessage() {}
    friend QDataStream &operator << (QDataStream &out, const RequestLoginMessage & msg){
        out << msg.id << msg.password;
        return out;
    }
    friend QDataStream &operator >> (QDataStream &in, RequestLoginMessage & msg){
        in >> msg.id >> msg.password;
        return in;
    }
};

struct LoginCheckMessage {
    Message::RequestStates state;
    QString hint, username;
    LoginCheckMessage(const Message::RequestStates &state, const QString &hint, const QString &username)
        :state(state), hint(hint), username(username)
    {}
    LoginCheckMessage() {}
    friend QDataStream &operator <<(QDataStream &out, const LoginCheckMessage &msg){
        out << msg.state << msg.hint << msg.username;
        return out;
    }
    friend QDataStream &operator >>(QDataStream &in, LoginCheckMessage &msg){
        in >> msg.state >> msg.hint >> msg.username;
        return in;
    }
};

struct ErrorMessage {
    QString errorMsg;
    ErrorMessage(const QString &errorMsg)
        :errorMsg(errorMsg)
    {}
    ErrorMessage() {}
    friend QDataStream &operator <<(QDataStream &out, const ErrorMessage &msg){
        out << msg.errorMsg;
        return out;
    }
    friend QDataStream &operator >>(QDataStream &in, ErrorMessage &msg){
        in >> msg.errorMsg;
        return in;
    }
};

struct RequestFriendRequestLogMessage{
    QString requestId;
    RequestFriendRequestLogMessage(const QString &requestId)
        :requestId(requestId) {}
    RequestFriendRequestLogMessage() {}
    friend QDataStream &operator <<(QDataStream &out,
                                    const RequestFriendRequestLogMessage &msg){
        out << msg.requestId;
        return out;
    }
    friend QDataStream &operator >>(QDataStream &in, RequestFriendRequestLogMessage &msg){
        in >> msg.requestId;
        return in;
    }
};

struct FriendRequestLogMessage{
    QString requestId;
    QList<FriendStatusMessage> friendStatusList;
    FriendRequestLogMessage(const QString &requestId,
                            const QList<FriendStatusMessage> &list):
        requestId(requestId)
    {
        for (int i = 0; i < list.length(); i++){
            friendStatusList.append(list.at(i));
        }
    }
    FriendRequestLogMessage(){}
    friend QDataStream &operator <<(QDataStream &out, const FriendRequestLogMessage &msg){
        out << msg.requestId << msg.friendStatusList.size();
        for (auto Statusmsg: msg.friendStatusList){
            out << Statusmsg;
        }
        return out;
    }
    friend QDataStream &operator >>(QDataStream &in, FriendRequestLogMessage &msg){
        qint32 sz;
        in >> msg.requestId >> sz;
        for (int i = 0; i < sz; i++) {
            FriendStatusMessage Statusmsg;
            in >> Statusmsg;
            msg.friendStatusList.append(Statusmsg);
        }
        return in;
    }
};

struct FileMessage{
    QString sendId, receiveId, fileName;
    QByteArray fileContent;
    FileMessage(const QString &sendId, const QString &receiveId, const QString &fileName,
                const QByteArray &fileContent):
        sendId(sendId), receiveId(receiveId), fileName(fileName), fileContent(fileContent)
    {

    }
    FileMessage(){}
    friend QDataStream &operator <<(QDataStream &out, const FileMessage &msg){
        out << msg.sendId << msg.receiveId << msg.fileName << msg.fileContent;
        return out;
    }
    friend QDataStream &operator >>(QDataStream &in, FileMessage &msg){
        in >> msg.sendId >> msg.receiveId >> msg.fileName >> msg.fileContent;
        return in;
    }
};


struct FindUserMessage {
    QString requestId;
    FindUserMessage(const QString &requestId)
        :requestId(requestId) {}
    FindUserMessage() {}
    friend QDataStream &operator <<(QDataStream &out, const FindUserMessage &msg){
        out << msg.requestId;
        return out;
    }
    friend QDataStream & operator >> (QDataStream &in, FindUserMessage &msg){
        in >> msg.requestId;
        return in;
    }
};


struct UserMessage {
    QString requestId; User user;
    UserMessage(const QString &requestId, const User &user)
        :requestId(requestId), user(user) {}
    UserMessage() {}
    friend QDataStream &operator <<(QDataStream &out, const UserMessage &msg){
        out << msg.requestId << msg.user;
        return out;
    }
    friend QDataStream & operator >> (QDataStream &in, UserMessage &msg){
        in >> msg.requestId << msg.user;
        return in;
    }
};


#endif // MESSAGE_H
