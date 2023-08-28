#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QTcpSocket>
#include <QDebug>
#include "message.h"
#include "client.h"
#include "login.h"
#define HOST_IP "10.194.45.92"
#define HOST_PORT 8888

class Communicator: public QObject
{
    Q_OBJECT
private:
    QTcpSocket *socket;

    void tryConnect();

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
