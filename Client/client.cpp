#include "client.h"
#include "ui_client.h"
#include <QDebug>
#include <QMouseEvent>
#include <QDataStream>
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
    ChatWindow *cw = new ChatWindow();
    chatWindowList.append(cw);
    QObject::connect(cw, &ChatWindow::signalSendMessageButtonClickedToClient, this, &Client::onSendMessageButtonFromChat);
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

void Client::onReadyReadFromCommunicator(const QByteArray &msg) {
    QByteArray dataSrc = msg;
    QDataStream dataStream(&dataSrc, QIODevice::ReadOnly);

    Message::Type type = Message::getType(dataStream);

    switch (type) {
    case Message::FRIENDLIST_MESSAGE:
    {
        struct FriendListMessage friendListMessage = Message::toFriendListMessage(dataStream);
        for(const User friendUser: friendListMessage.friendList) {
//			addChat(friendUser.id, friendUser.username);
        }
        break;
    }
    default:
//        emit signalReadyReadToChat(msg);
        break;
    }
}

void Client::rcvLogin(QString str,QString str1){
    qDebug()<<"reghvbjdfbvjvdskvsf"<<endl;
//    update(str,str1);
    emit closeLoginWindow();
    show();
}
void Client::update(const QByteArray &dataSrc){
//    qDebug()<<"00000"<<a<<b<<endl;
//    ui->usernameLabel->setText("这是"+a);
//    ui->signature->setText("这是"+a+"的签名");
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
    qDebug() << pos() << e->pos() << m_ptPress;
    m_bPressed = m_areaMovable.contains(m_ptPress);
  }
}

void Client::mouseMoveEvent(QMouseEvent *e)
{
  if(m_bPressed)
  {
    qDebug() << pos() << e->pos() << m_ptPress;
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


