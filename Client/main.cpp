#include "client.h"

#include <QApplication>
#include "chatwindow.h"
#include "communicator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client w;
    w.show();

    Communicator communicator;
    communicator.initCommunicator(w);
//    ChatWindow w;
//    w.show();
    return a.exec();
}
