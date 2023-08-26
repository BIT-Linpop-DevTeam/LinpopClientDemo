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

//    ChatWindow w;
//    w.show();


    return a.exec();
}
