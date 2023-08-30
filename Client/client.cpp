#include "client.h"
#include "ui_client.h"
#include <QDebug>
#include <QMouseEvent>
#include <QDataStream>
#include "acceptfriend.h"
#include "mylog.h"
#include <QFileDialog>
//#include<login.h>
#include<userprofile.h>
Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{//
    ui->setupUi(this);
    ui->signature->setText("这个人很懒，还没有任何签名");
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
            qDebug() << QString("ADD in friend list: %1 %2 %3").arg(friendUser.id).arg(friendUser.username).arg(friendUser.avatar);
            addChat(userId, username, avatarId, friendUser.id, friendUser.username, friendUser.avatar);

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
        qDebug() << QString("received requests friend in client: userId %1 friendId %2").arg(requestFriendMessage.friendId).arg(requestFriendMessage.requestId);
        if(requestFriendMessage.friendId != userId) return;
        if(requestFriendMessage.states == Message::SUCCESS) {
            RequestFriendListMessage requestFriendListmessage(this->userId);
            emit signalSendMessageToCommunicator(Message::FromRequestFriendListMessage(requestFriendListmessage));
        } else if(requestFriendMessage.states == Message::UNTREATED) {
            Acceptfriend *acceptFriendWindow = new Acceptfriend(nullptr, requestFriendMessage.friendId, requestFriendMessage.requestId);
            qDebug() << QString("received UNRATED request friend: requestId %1 friendId %2").arg(requestFriendMessage.requestId).arg(requestFriendMessage.friendId);
            acceptFriendWindow->init();
            connect(acceptFriendWindow, &Acceptfriend::signalRequestFriendMessageToClient, this, &Client::onSendMessageFromChildToCommunitor);
            acceptFriendWindow->show();
            qDebug() << "show accept window";
        } else {
            qDebug() << "in slot: onReadyReadFromCommunicator. unexpected fail state in requestFriend msg";
        }
        break;
    }
    case Message::ERROR_MESSAGE:
    {
        ErrorMessage errorMessage = Message::toErrorMessage(dataStream);
        myLog aa(myLog::Critical, "错误信息", errorMessage.errorMsg, myLog::Ok);
        break;
    }
    default:
        emit signalReadyReadToChat(msg);
        break;
    }
}

void Client::initClient() {
    qDebug() << "client inited";
//    this->t("1", "user1", "nihao");
//    this->t("2", "用户2");
    struct RequestFriendListMessage requestFriendListMessage(userId);
    QByteArray msg = Message::FromRequestFriendListMessage(requestFriendListMessage);
    ui->verticalLayout_6->setAlignment(Qt::AlignTop);
    emit signalSendMessageToCommunicator(msg);

    addFriendWindow = new addFriend(nullptr, userId, username);
    addFriendWindow->initAddFriend();
//    connect(ui->messagepushButton,&QPushButton::clicked,this,&Client::switchPage);
//    connect(ui->friendspushButton,&QPushButton::clicked,this,&Client::switchPage);
    connect(addFriendWindow, &addFriend::signalRequestFriendToClient, this, &Client::onSendMessageFromChildToCommunitor);
    connect(ui->addFriBotton, &QPushButton::clicked, this, &Client::onAddFriendButtonClicked);
}

void Client::rcvLogin(QString userId, QString username, qint32 avatarId){

    qDebug()<<"in slot: rcvLogin"<<endl;
    update(userId, username, avatarId);
    emit closeLoginWindow();
    initClient();
    show();
}

void Client::switchPage(){
//    QPushButton *button = qobject_cast<QPushButton*>(sender());//得到按下的按钮的指针
//    if(button==ui->messagepushButton)
//        ui->stackedWidget->setCurrentIndex(0);//根据按下的button按索引显示相应的页面
//    else if(button==ui->friendspushButton)
//        ui->stackedWidget->setCurrentIndex(1);

}

void Client::update(const QString &userId, const QString &username, qint32 avatarId){
    this->userId = userId;
    this->username = username;
    this->avatarId = avatarId;
    qDebug()<<"00000"<<username<<endl;
    ui->usernameLabel->setText(username);
    qDebug() << "in update: avatarId = " << avatarId;
    ui->avatarLabel->setStyleSheet(QString("border-image: url(:/src/GUI/head/%1.jpg);").arg(avatarId));
}

void Client::on_closeButton_clicked()
{
    for(auto cw: chatWindowList) {
        cw->close();
    }
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

bool f=1;

void Client::addChat(const QString &ownerId, const QString &ownername, const qint32 &ownerAvatar , const QString &userId, const QString &username, const qint32 &userAvatar) {
    qDebug() << "in function: addChat, ownerId = " << ownerId << " userID = " << userId;
    if(idSet.contains(userId)) return;
    idSet.insert(userId);


       QString Filepath;
       Filepath =QDir::currentPath ();
       //qDebug () << Filepath;
       int index = Filepath.lastIndexOf("/");
           Filepath = Filepath.left(index);
           //qDebug()<<"path=="<<Filepath;
           Filepath+="/Client/src/GUI/head";

           Filepath+='/';
           Filepath+=QString::number(userAvatar);
           Filepath+=".jpg";
           qDebug()<<"path=="<<Filepath;
           QString dirPath_ = Filepath;
           QListWidgetItem *pItem = new QListWidgetItem;
           pItem->setSizeHint(QSize(60,60));
           pItem->setText(Filepath);
           pItem->setIcon(QIcon(QPixmap(Filepath).scaled(QSize(60,60))));
           //ui->listWidget->addItem(pItem);
    qDebug() << QString("in addChat, userId = %1, username = %2").arg(userId).arg(username);
    QPushButton *friendButton = new QPushButton();
    QPixmap icon1(Filepath);
    friendButton->setIcon(icon1);
    friendButton-> setIconSize ( QSize ( 55, 55 ));
    friendButton->setFixedSize (QSize(ui->friendSearchEdit->rect().width()+ui->label->rect().width(),60));
    friendButton->setStyleSheet("QPushButton{ background-color: rgb(240, 240, 240,130);spacing: 25px;height:50px;font: '微软雅黑';font-size: 13pt;color:rgb(0,0,0); }"
                           "QPushButton:hover{background-color: rgb(219, 219, 219,200);spacing: 50px;font: '微软雅黑';font-size: 13pt;color:rgb(0,0,0);}"
                           "QPushButton:press{background-color: rgb(219, 219, 219,200);spacing: 25px;font: '微软雅黑';font-size: 13pt;color:rgb(0,0,0);}"
                           "QPushButton:indicator{width: 0px;height: 0px;border: none;}"
                              "");
    friendButton->setAutoFillBackground(1);
    //ui->scrollAreaWidgetContents_2->setStyleSheet()
    ui->verticalLayout_6->addWidget(friendButton, 0, Qt::AlignTop);
    friendButton->setText(username);
    friendButton->setLayoutDirection(Qt::LeftToRight);
    ChatWindow *cw = new ChatWindow(nullptr, ownerId, ownername, ownerAvatar, userId, username, userAvatar);
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


void Client::on_modeBotton_clicked()
{
    f^=1;
    if(f){
        ui->modeBotton->setStyleSheet("#modeBotton{image: url(:/src/GUI/icon/nightmode.png);border:none;}"
                                     "#modeBotton:hover{"
                                      "image: url(:/src/GUI/icon/nightmode_hover.png);"
                                      "border:none;"
                                     "}"
                                     "#modeBotton:pressed{"
                                      "image: url(:/src/GUI/icon/nightmode_hover.png);"
                                      "border:none;"
                                     "}");
        ui->scrollAreaWidgetContents_3->setStyleSheet("*{background-color: rgba(255, 255, 255,100);}");
        ui->scrollAreaWidgetContents_2->setStyleSheet("*{background-color: rgba(255, 255, 255,100);}");
        for(auto x:chatWindowList){
           x->changeMode(1);
       }
    }
    else{
        ui->modeBotton->setStyleSheet("#modeBotton{image: url(:/src/GUI/icon/daymode.png);border:none;}"
                                     "#modeBotton:hover{"
                                      "image: url(:/src/GUI/icon/daymode_hover.png);"
                                      "border:none;"
                                     "}"
                                     "#modeBotton:pressed{"
                                      "image: url(:/src/GUI/icon/daymode_hover.png);"
                                      "border:none;"
                                     "}");
        ui->scrollAreaWidgetContents_3->setStyleSheet("*{background-color: rgba(0, 0, 0,100);}");
        ui->scrollAreaWidgetContents_2->setStyleSheet("*{background-color: rgba(0, 0, 0,100);}");
        for(auto x:chatWindowList){
            x->changeMode(0);
        }

    }
}

void Client::on_moreButton_clicked()
{
    UserProfile* x=new UserProfile;
    connect(x, &UserProfile::signalConfirmUsernameToClient, this, &Client::onConfirmUsernameFromChange);
    connect(x, &UserProfile::signalConfirmSignatureToClient, this, &Client::onConfirmSignatureFromChange);
    connect(x, &UserProfile::signalConfirmAvatarToClient, this, &Client::onConfirmAvatarFromChange);
    x->show();
    //qDebug()<<"ffff";
}

void Client::onConfirmUsernameFromChange(const QString &username) {
    update(userId, username, avatarId);
    User user(userId, username, avatarId);
    UserMessage userMessage(userId, user);
    emit signalSendMessageToCommunicator(Message::FromUserMessage(userMessage));
}

void Client::onConfirmSignatureFromChange(const QString &signature) {
    ui->signature->setText(signature);
}

void Client::onConfirmAvatarFromChange(const qint32 &avatarId) {
    update(userId, username, avatarId);
    User user(userId, username, avatarId);
    UserMessage userMessage(userId, user);
    emit signalSendMessageToCommunicator(Message::FromUserMessage(userMessage));
}
