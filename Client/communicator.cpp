#include "communicator.h"
#include <QHostAddress>
#define MAX_CONNECT_TIMES 3

Communicator::Communicator()
{
    socket = new QTcpSocket();
    tryConnect();
    QObject::connect(socket, &QTcpSocket::readyRead, this, &Communicator::onReadyReadFromSocket);
}

Communicator::~Communicator() {
    socket->abort();
    delete socket;
}

void Communicator::tryConnect() {
    for(int i = 0; i < MAX_CONNECT_TIMES; i++) {
        socket->connectToHost(QHostAddress(HOST_IP), HOST_PORT);
        if(socket->waitForConnected())	{
            qDebug() << "connected";
            return;
        }
    }
    qDebug() << "connect fail";
}

void Communicator::initCommunicator(const Client &client) {
    QObject::connect(&client, &Client::signalSendMessageToCommunicator, this, &Communicator::onSendMessageClickedFromClient);
    QObject::connect(socket, &QTcpSocket::connected, this, &Communicator::onConnectedFromSocket);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &Communicator::onDisconnectedFromSocket);
}

void Communicator::onConnectedFromSocket() {
    qDebug() << "connected";
}

void Communicator::onDisconnectedFromSocket() {
    qDebug() << "disconnected";
    tryConnect();
}

void Communicator::onReadyReadFromSocket() {
    qDebug() << "in slot: onReadyReadFromSocket()";

    if(socket->bytesAvailable() <= 0) 	return;

    const QByteArray recvData = socket->readAll();
    const Message msg(recvData);

    emit signalReadyReadToClient(msg);
}

void Communicator::onSendMessageClickedFromClient(const Message &msg) {
    qDebug() << "in slot: onSendMessageClickedFromClient()";

    if(!socket->isValid())	{
        qDebug() << "send failed!";
        return;
    }

    const QByteArray sendData = msg.toByteArray();
    socket->write(sendData);
}
