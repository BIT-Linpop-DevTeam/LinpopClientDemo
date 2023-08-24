#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <qDebug>

ChatWindow::ChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->sendPushButton, &QPushButton::clicked, this, &ChatWindow::onSendMessageButtonClicked);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::onReadyReadFromClient(const Message& msg)
{
    qDebug() << "in slot: onReadySendFromClient()";
    const QString text = msg.toString();
    ui->msgShowTextBrowser->append(text);
}

void ChatWindow::onCreateWindowButtonClickedFromClient()
{
    qDebug() << "in slot: onButtonClickedFromClient()";
    show();
}

void ChatWindow::onSendMessageButtonClicked()
{
    qDebug() << "in slot: onSendMessageButtonClicked()";
    const QString sendData = ui->msgSendTextEdit->toPlainText();
    ui->msgShowTextBrowser->append("me:");
    ui->msgShowTextBrowser->append(sendData);
    Message msg(sendData);
    emit signalSendMessageButtonClickedToClient(msg);
}
