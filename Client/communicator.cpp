#include "communicator.h"
#include <QHostAddress>
#include <QTextStream>
#include <QDir>
#define MAX_CONNECT_TIMES 3
#define FILENAME "HOST.txt"

Communicator::Communicator()
{
    socket = new QTcpSocket();
    tryConnect();
    QObject::connect(socket, &QTcpSocket::readyRead, this, &Communicator::onReadyReadFromSocket);
    bytesReceived = 0;
    totalBytes = 0;
    receiveContent.clear();
    finalContent.clear();

    QDir dir = QDir::current();
    QString FilePath = dir.filePath(FILENAME);
    QFile file(FilePath);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "open host file fail";
    }
    QTextStream fileStream(&file);

    fileStream >> HOST_IP;
    fileStream >> HOST_PORT;
    qDebug() << "host ip: " <<  HOST_IP << " host port: " << HOST_PORT;
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

    QDataStream in(socket);

    if(bytesReceived < (int)sizeof(qint64)){
        if(socket->bytesAvailable() >= (int)sizeof(qint64) && totalBytes == 0){
            in >> totalBytes;
            bytesReceived += sizeof(totalBytes);
        }
        //如果现在连文件大小都没传过来，给爹候着。
        else return;
    }
    // 现在将要读的大小
    qint64 readBlockSize;
    if(bytesReceived < totalBytes){
        readBlockSize =qMin(socket->bytesAvailable(), totalBytes - bytesReceived);
        receiveContent.append(socket->read(readBlockSize));
        bytesReceived += readBlockSize;
    }
    if(bytesReceived == totalBytes){
//        finalContent = receiveContent;
        QDataStream inSecond(receiveContent);
        inSecond >> finalContent;
        // 发送准备读取消息信号到客户端
        emit signalReadyReadToClient(finalContent);

        // 清空已经读到的内容
        receiveContent.clear();
        finalContent.clear();
        totalBytes = 0;
        bytesReceived = 0;
        if (socket->bytesAvailable() >= (int)sizeof(qint64) ){
            onReadyReadFromSocket();
        }
    }


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

    // 发送消息到服务器
    QByteArray byte;
    QDataStream numOut(&byte, QIODevice::WriteOnly);

    // 先占位 + 输出msg
    numOut << qint64(0) << msg;

    // 指针回到零位置
    numOut.device()->seek(0);

    // 将占位符顶掉
    numOut << qint64(byte.size());

    qDebug() << "发送消息的大小（包括qint64） " << byte.size();
    // 发送到服务器
    socket->write(byte);
    socket->flush();
}

void Communicator::onRequestLoginFromLogin(const QByteArray &msg) {
     qDebug() << "in slot: onRequestLoginFromLogin";

     QByteArray dataSrc = msg;
     QDataStream dataStream(msg);
     qDebug() << "Message type: " << Message::getType(dataStream);

     if(socket->state() == QTcpSocket::UnconnectedState)  tryConnect();
     if(!socket->isValid()) {
         qDebug() << "send login request failed";
         return;
     }
     // 发送消息到服务器
     QByteArray byte;
     QDataStream numOut(&byte, QIODevice::WriteOnly);

     // 先占位 + 输出msg
     numOut << qint64(0) << msg;

     // 指针回到零位置
     numOut.device()->seek(0);

     // 将占位符顶掉
     numOut << qint64(byte.size());

     qDebug() << "发送消息的大小（包括qint64） " << byte.size();
     socket->write(byte);
     socket->flush();

     QDataStream dataS(byte);
     qint64 x;
     QByteArray byteType;
     Message::Type type;
     dataS >> x >> byteType;
     QDataStream dataS2(byteType);
     dataS2 >> type;
     qDebug() << x << " " << type;
}
