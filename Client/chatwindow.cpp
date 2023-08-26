#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QDebug>
#include <QDateTime>

ChatWindow::ChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->sendPushButton, &QPushButton::clicked, this, &ChatWindow::onSendMessageButtonClicked);
}

ChatWindow::ChatWindow(QWidget *parent, const QString &ownerId, const QString &userId, const QString &username)
    : ChatWindow(parent)
{
    this->ownerId = ownerId;
    this->userId = userId;
    this->username = username;
    ui->friendNameLabel->setText(username);
    setWindowTitle(QString("和%1的对话").arg(username));
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

//todo
void ChatWindow::onReadyReadFromClient(const QByteArray& msg)
{
    qDebug() << "in slot: onReadySendFromClient()";

    QByteArray dataSrc = msg;
    QDataStream dataStream(&dataSrc, QIODevice::ReadOnly);
    Message::Type type = Message::getType(dataStream);

    switch (type) {
    case Message::CHAT_MESSAGE:
    {
        const ChatMessage chatMsg = Message::toChatMessage(dataStream);


        //todo :diff the output way of me and counterpart
        if(chatMsg.receiveId == userId) {
            ui->msgShowTextBrowser->append(QString("%1(我):").arg(chatMsg.sendId));
            ui->msgShowTextBrowser->append(chatMsg.msg);
        } else if(chatMsg.sendId == userId) {
            ui->msgShowTextBrowser->append(QString("%1(对方):").arg(chatMsg.sendId));
            ui->msgShowTextBrowser->append(chatMsg.msg);
        }
        break;
    }
    case Message::REQUEST_FRIENDLIST_MESSAGE:
        break;
    case Message::FRIENDLIST_MESSAGE:
        break;
    case Message::REQUEST_CHATLOG_MESSAGE:
        break;
    case Message::CHATLOG_MESSAGE:
        break;
    case Message::REQUEST_FRIEND_MESSAGE:
        break;
    case Message::REQUEST_LOGIN_MESSAGE:
        break;
    case Message::LOGIN_CHECK_MESSAGE:
        break;
    case Message::REQUEST_SIGNUP_MESSAGE:
        break;
    case Message::SIGNUP_CHECK_MESSAGE:
        break;
    case Message::ERROR_MESSAGE:
        break;
    default:
        break;
    }
}

void ChatWindow::onCreateWindowButtonClickedFromClient()
{
    qDebug() << "in slot: onButtonClickedFromClient()";
    show();
}

//todo
void ChatWindow::onSendMessageButtonClicked()
{
    qDebug() << "in slot: onSendMessageButtonClicked()";
    const QString sendData = ui->msgSendTextEdit->toPlainText();

    if(sendData.length() <= 0)	return;

    ui->msgSendTextEdit->setText("");
    ui->msgShowTextBrowser->append("me:");
    ui->msgShowTextBrowser->append(sendData);

    QString sendId = ownerId, receiveId = this->userId;
    struct ChatMessage dataSrc(sendId, receiveId, sendData, QDateTime::currentDateTime());
    const QByteArray msg = Message::FromChatMessage(dataSrc);
    emit signalSendMessageButtonClickedToClient(msg);
}
