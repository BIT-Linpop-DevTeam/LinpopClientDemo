#include "acceptfriend.h"
#include "ui_acceptfriend.h"
#include "message.h"

Acceptfriend::Acceptfriend(QWidget *parent, const QString &userId, const QString &friendId) :
    QWidget(parent),
    ui(new Ui::Acceptfriend), userId(userId), friendId(friendId)
{
    ui->setupUi(this);
    ui->usernameLabel->setText(friendId);
}

Acceptfriend::~Acceptfriend()
{
    delete ui;
}

void Acceptfriend::onConfirmButtonClicked()
{
    RequestFriendMessage requestFriendMessage(userId, friendId, Message::SUCCESS);
    emit signalRequestFriendMessageToClient(Message::FromRequestFriendMessage(requestFriendMessage));
    RequestFriendListMessage requestFriendListMessage(userId);
    emit signalRequestFriendMessageToClient(Message::FromRequestFriendListMessage(requestFriendListMessage));
    close();
}

void Acceptfriend::onCancelButtonClicked()
{
    RequestFriendMessage requestFriendMessage(userId, friendId, Message::FAIL);
    emit signalRequestFriendMessageToClient(Message::FromRequestFriendMessage(requestFriendMessage));
    close();
}

void Acceptfriend::init()
{
    connect(ui->acceptButton, &QPushButton::clicked, this, &Acceptfriend::onConfirmButtonClicked);
    connect(ui->denyButton, &QPushButton::clicked, this, &Acceptfriend::onCancelButtonClicked);
}
