#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    setWindowTitle("friends list");
    ChatWindow *cw = new ChatWindow();
    chatWindowList.append(cw);
    QObject::connect(cw, &ChatWindow::signalSendMessageButtonClickedToClient, this, &Client::onSendMessageButtonFromChat);
    QObject::connect(ui->addChatWindow, &QPushButton::clicked, cw, &ChatWindow::onCreateWindowButtonClickedFromClient);
}

Client::~Client()
{
    delete ui;
}

void Client::onSendMessageButtonFromChat(const QByteArray &msg) {
    emit signalSendMessageToCommunicator(msg);
}
