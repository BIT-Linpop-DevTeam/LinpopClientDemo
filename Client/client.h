#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QByteArray>
#include "message.h"
#include "chatwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

    //to be done
    void initClient();

private:
    Ui::Client *ui;
    QList<ChatWindow *> chatWindowList;

signals:
    void signalSendMessageToCommunicator(const QByteArray &msg);

public slots:
    void onSendMessageButtonFromChat(const QByteArray &msg);
};
#endif // CLIENT_H
