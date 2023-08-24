#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QTcpSocket>
#include <QDebug>
#include "message.h"
#include "client.h"
#define HOST_IP "127.0.0.1"
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
    void initCommunicator(const Client &client);

signals:
    void signalReadyReadToClient(const Message &msg);

public slots:
    void onSendMessageClickedFromClient(const Message &msg);
    void onReadyReadFromSocket();
    void onConnectedFromSocket();
    void onDisconnectedFromSocket();
};

#endif // COMMUNICATOR_H
