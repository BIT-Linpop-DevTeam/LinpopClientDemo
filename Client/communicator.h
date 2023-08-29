#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QTcpSocket>
#include <QDebug>
#include "message.h"
#include "client.h"
#include "login.h"
#define HOST_IP "10.194.39.19"
#define HOST_PORT 8888

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
public:
    Communicator();
    ~Communicator();
    void initCommunicator(const Client &client, const Login &login);

signals:
    void signalReadyReadToClient(const QByteArray &msg);

public slots:
    void onSendMessageClickedFromClient(const QByteArray &msg);
    void onReadyReadFromSocket();
    void onConnectedFromSocket();
    void onDisconnectedFromSocket();
    void onRequestLoginFromLogin(const QByteArray &msg);
};

#endif // COMMUNICATOR_H
