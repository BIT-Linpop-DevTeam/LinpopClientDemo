#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QTcpSocket>
#include <QDebug>
#include "message.h"
#include "client.h"
#include "login.h"

class Communicator: public QObject
{
    Q_OBJECT
private:
    QTcpSocket *socket;

    void tryConnect();

    qint64 totalBytes;    //将要读的数据总大小
   qint64 bytesReceived; //已经读完的数据大小
   // 现在接到的部分 二进制数组
   QByteArray receiveContent;
   // 接收完成的 二进制数组
   QByteArray finalContent;
   QString HOST_IP;
   qint16 HOST_PORT;
public:
    Communicator();
    ~Communicator();
    /**
     * @brief initCommunicator 用于初始化通讯类的函数，负责通讯类与客户端、登录类之间信号与槽的连接
     * @param client 客户端
     * @param login  登录类
     */
    void initCommunicator(const Client &client, const Login &login);

signals:
    /**
     * @brief signalReadyReadToClient 信号，收到信息时通知客户端，由客户端统一处理
     * @param msg 由Message方法类将消息转化格式后得到的二进制数据
     */
    void signalReadyReadToClient(const QByteArray &msg);

public slots:
    /**
     * @brief onSendMessageClickedFromClient 槽函数，从客户端得到需要发送的消息后，由通讯类进行发送
     * @param msg
     */
    void onSendMessageClickedFromClient(const QByteArray &msg);
    void onReadyReadFromSocket();
    void onConnectedFromSocket();
    void onDisconnectedFromSocket();
    /**
     * @brief onRequestLoginFromLogin 槽函数，从登录类得到需要发送的消息后，由通讯类进行发送
     * @param msg
     */
    void onRequestLoginFromLogin(const QByteArray &msg);
};

#endif // COMMUNICATOR_H
