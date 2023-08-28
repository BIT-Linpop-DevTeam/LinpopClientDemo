#include "client.h"

#include <QApplication>
#include "chatwindow.h"
#include "communicator.h"
#include "login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client client;
    Login login;

    Communicator communicator;
    communicator.initCommunicator(client, login);

    login.initLogin(client);

//    client.show();

//    QByteArray data;
//    QDataStream dataStream(&data, QIODevice::WriteOnly);
//    dataStream << (qint64)18;
//    dataStream << (qint64)1000000;
//    dataStream << (qint32)10;

//    qDebug() << data.size();

//    QByteArray data2 = data.chopped(12);

//    QDataStream dataS2(&data2, QIODevice::ReadOnly);
//    qint64 x;
//    dataS2 >> x;
//    qDebug() << x;


    return a.exec();
}
