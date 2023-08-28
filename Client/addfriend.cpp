#include "addfriend.h"
#include "ui_addfriend.h"
#include "mylog.h"
#include "message.h"

addFriend::addFriend(QWidget *parent, const QString &userId, const QString &username) :
    QWidget(parent),
    ui(new Ui::addFriend), userId(userId), username(username)
{
    ui->setupUi(this);
}

addFriend::~addFriend()
{
    delete ui;
}

void addFriend::onCancelButtonClicked() {
    ui->usernameEdit->setText("");
    close();
}

void addFriend::onConfirmButtonClicked() {
    QString requestId = ui->usernameEdit->text();
    ui->usernameEdit->setText("");

    if(requestId.length() <= 0) {
        myLog aa(myLog::Critical, "提示", "非法的用户ID", myLog::Ok);
        aa.show();
        aa.exec();
        return;
    }

    RequestFriendMessage requestFriendMessage(userId, requestId, Message::UNTREATED);
    emit signalRequestFriendToClient(Message::FromRequestFriendMessage(requestFriendMessage));
    close();

    myLog aa(myLog::Critical, "提示", "好友申请已发送", myLog::Ok);
    aa.show();
    aa.exec();
}

void addFriend::initAddFriend() {
   connect(ui->confirmButton, &QPushButton::clicked, this, &addFriend::onConfirmButtonClicked);
   connect(ui->cancelButton, &QPushButton::clicked, this, &addFriend::onCancelButtonClicked);
//   connect(this, &addFriend::signalRequestFriendToClient, &client, &Client::onSendMessageButtonFromChat);
}
