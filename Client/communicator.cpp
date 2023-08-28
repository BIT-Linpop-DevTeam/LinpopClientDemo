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
        if(socket->waitForConnected(500))	{
            qDebug() << "connected";
            return;
        }
    }
    qDebug() << "connect fail";
}

void Communicator::initCommunicator(const Client &client, const Login &login) {
    QObject::connect(this, &Communicator::signalReadyReadToClient, &client, &Client::onReadyReadFromCommunicator);
    QObject::connect(&client, &Client::signalSendMessageToCommunicator, this, &Communicator::onSendMessageClickedFromClient);
    QObject::connect(socket, &QTcpSocket::connected, this, &Communicator::onConnectedFromSocket);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &Communicator::onDisconnectedFromSocket);
    QObject::connect(&login, &Login::signalRequestLoginToCommunicator, this, &Communicator::onRequestLoginFromLogin);
}

void Communicator::onConnectedFromSocket() {
    qDebug() << "connected";
}

void Communicator::onDisconnectedFromSocket() {
    qDebug() << "disconnected";
//    tryConnect();
}

void Communicator::onReadyReadFromSocket() {
    qDebug() << "in slot: onReadyReadFromSocket()";

    if(socket->bytesAvailable() <= 0) 	return;

    const QByteArray recvData = socket->readAll();

    emit signalReadyReadToClient(recvData);
}

void Communicator::onSendMessageClickedFromClient(const QByteArray &msg) {
    qDebug() << "SEND MESSAGE. in slot: onSendMessageClickedFromClient()";

    QByteArray dataSrc = msg;
    QDataStream dataStream(&dataSrc, QIODevice::ReadOnly);

    qDebug() << "send message type: " << Message::getType(dataStream);

    if(socket->state() == QTcpSocket::UnconnectedState)  tryConnect();
    if(!socket->isValid())	{
        qDebug() << "send failed!";
        return;
    }

    qDebug() << socket->write(msg);
    socket->flush();
    qDebug() << "send success";
}

void Communicator::onRequestLoginFromLogin(const QByteArray &msg) {
     qDebug() << "in slot: onRequestLoginFromLogin";

     if(socket->state() == QTcpSocket::UnconnectedState)  tryConnect();
     if(!socket->isValid()) {
         qDebug() << "send login request failed";
         return;
     }

     socket->write(msg);
     socket->flush();
}
