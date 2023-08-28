#include "client.h"
#include "ui_client.h"
#include <QDebug>
#include <QMouseEvent>
#include <QDataStream>
#include "acceptfriend.h"
//#include<login.h>
Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{//
    ui->setupUi(this);
    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    //把窗口背景设置为透明;
    setAttribute(Qt::WA_TranslucentBackground);
    m_areaMovable = geometry();
    m_bPressed = false;
    setWindowTitle("friends list");

//    ChatWindow *cw = new ChatWindow();
//    chatWindowList.append(cw);
//    QObject::connect(cw, &ChatWindow::signalSendMessageButtonClickedToClient, this, &Client::onSendMessageButtonFromChat);
//    QObject::connect(ui->FriendButton1, &QPushButton::clicked, cw, &ChatWindow::onCreateWindowButtonClickedFromClient);
//    QObject::connect(this, &Client::updateFriendName, cw, &ChatWindow::updateCreateWindow);
}

Client::~Client()
{
    delete ui;
}

void Client::onSendMessageButtonFromChat(const QByteArray &msg) {
    emit signalSendMessageToCommunicator(msg);
}

void Client::onSendMessageFromChildToCommunitor(const QByteArray &msg) {
    emit signalSendMessageToCommunicator(msg);
}

void Client::onReadyReadFromCommunicator(const QByteArray &msg) {
    qDebug() << "in slot: onReadyReadFromCommunicator";

    QByteArray dataSrc = msg;
    QDataStream dataStream(&dataSrc, QIODevice::ReadOnly);

    Message::Type type = Message::getType(dataStream);
    qDebug() << "Message type: " << type;

    switch (type) {
    case Message::FRIENDLIST_MESSAGE:
    {
        struct FriendListMessage friendListMessage = Message::toFriendListMessage(dataStream);
        for(const User friendUser: friendListMessage.friendList) {
            qDebug() << QString("add in friend list: %1 %2").arg(friendUser.id).arg(friendUser.username);
            addChat(userId, friendUser.id, friendUser.username);

            RequestChatLogMessage requestChatLogMessage(userId, friendUser.id);
            QByteArray dataSrc = Message::FromRequestChatLogMessage(requestChatLogMessage);
            emit signalSendMessageToCommunicator(dataSrc);
        }
        break;
    }
    case Message::SIGNUP_CHECK_MESSAGE:
    case Message::LOGIN_CHECK_MESSAGE:
        emit signalLoginCheckToLogin(msg);
        break;
    case Message::REQUEST_FRIEND_MESSAGE:
    {
        RequestFriendMessage requestFriendMessage = Message::toRequestFriendMessage(dataStream);
        if(requestFriendMessage.states == Message::SUCCESS) {
            RequestFriendListMessage requestFriendListmessage(this->userId);
            emit signalSendMessageToCommunicator(msg);
        } else if(requestFriendMessage.states == Message::UNTREATED) {
            Acceptfriend *acceptFriendWindow = new Acceptfriend(nullptr, requestFriendMessage.friendId, requestFriendMessage.requestId);
            acceptFriendWindow->show();
            qDebug() << "show accept window";
        } else {
            qDebug() << "in slot: onReadyReadFromCommunicator. unexpected fail state in requestFriend msg";
        }
        break;
    }
    default:
        emit signalReadyReadToChat(msg);
        break;
    }
}

//todo
void Client::initClient() {
    qDebug() << "client inited";
//    this->addChat("1", "user1");
//    this->addChat("2", "用户2");
    struct RequestFriendListMessage requestFriendListMessage(userId);
    QByteArray msg = Message::FromRequestFriendListMessage(requestFriendListMessage);
    ui->verticalLayout_6->setAlignment(Qt::AlignTop);
    emit signalSendMessageToCommunicator(msg);

    addFriendWindow = new addFriend(nullptr, userId, username);
    addFriendWindow->initAddFriend();
    connect(addFriendWindow, &addFriend::signalRequestFriendToClient, this, &Client::onSendMessageFromChildToCommunitor);
    connect(ui->addFriBotton, &QPushButton::clicked, this, &Client::onAddFriendButtonClicked);
}

//todo
void Client::rcvLogin(QString userId, QString username){

    qDebug()<<"in slot: rcvLogin"<<endl;
    update(userId, username);
    emit closeLoginWindow();
    initClient();
    show();
}

//todo
void Client::update(const QString &userId, const QString &username){
    this->userId = userId;
    this->username = username;
    qDebug()<<"00000"<<username<<endl;
    ui->usernameLabel->setText("这是"+username);
    ui->signature->setText("这是"+username+"的签名");
}

void Client::on_closeButton_clicked()
{
    close();
}

void Client::on_minimizeButton_clicked()
{
    showMinimized();
}
void Client::mousePressEvent(QMouseEvent *e)
{
  //鼠标左键
  if(e->button() == Qt::LeftButton)
  {
    m_ptPress = e->pos();
//    qDebug() << pos() << e->pos() << m_ptPress;
    m_bPressed = m_areaMovable.contains(m_ptPress);
  }
}

void Client::mouseMoveEvent(QMouseEvent *e)
{
  if(m_bPressed)
  {
//    qDebug() << pos() << e->pos() << m_ptPress;
    move(pos() + e->pos() - m_ptPress);
  }
}

void Client::mouseReleaseEvent(QMouseEvent *)
{
  m_bPressed = false;
}

//设置鼠标按下的区域
void Client::setAreaMovable(const QRect rt)
{
  if(m_areaMovable != rt)
  {
  m_areaMovable = rt;
  }
}

void Client::addChat(const QString &ownerId, const QString &userId, const QString &username) {
   qDebug() << QString("in addChat, userId = %1, username = %2").arg(userId).arg(username);
   QPushButton *friendButton = new QPushButton();
   friendButton->setStyleSheet("QPushButton{ background-color: rgb(240, 240, 240);spacing: 25px; }"
                            "QPushButton:hover{background-color: rgb(219, 219, 219);spacing: 25px;}"
                            "QPushButton:press{background-color: rgb(219, 219, 219);spacing: 25px;}"
                            "QPushButton:indicator{width: 0px;height: 0px;border: none;}");
   ui->verticalLayout_6->addWidget(friendButton, 0, Qt::AlignTop);
   friendButton->setText(username);
   ChatWindow *cw = new ChatWindow(nullptr, ownerId, userId, username);
   chatWindowList.append(cw);

   QObject::connect(friendButton, &QPushButton::clicked, cw, &ChatWindow::onCreateWindowButtonClickedFromClient);
   QObject::connect(this, &Client::signalReadyReadToChat, cw, &ChatWindow::onReadyReadFromClient);
   QObject::connect(cw, &ChatWindow::signalSendMessageButtonClickedToClient, this, &Client::onSendMessageButtonFromChat);
}

void Client::onAddFriendButtonClicked()
{
    qDebug() << "in slot: onAddFriendButtonClicked";
    addFriendWindow->show();
}



