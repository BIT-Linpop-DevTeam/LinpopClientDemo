#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#include <QQmlApplicationEngine>
#include <QFileInfo>
#include <QMouseEvent>

#define BLOCK_SIZE 20
void writeFileFromQByteArray(const QString &fileDir, const QString &fileName,  const QByteArray &data, const Message::FileState &fileState);

ChatWindow::ChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    //把窗口背景设置为透明;
    setAttribute(Qt::WA_TranslucentBackground);

    m_areaMovable = geometry();
    m_bPressed = false;

    QObject::connect(ui->sendPushButton, &QPushButton::clicked, this, &ChatWindow::onSendMessageButtonClicked);
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    QObject::connect(ui->sendPushButton, &QPushButton::clicked, this, &ChatWindow::onSendMessageButtonClicked);

    qmlRegisterType<EmojiModel>("EmojiModel", 1, 0, "EmojiModel");
    engine = new QQmlApplicationEngine();


}



ChatWindow::ChatWindow(QWidget *parent, const QString &ownerId, const QString &ownername, qint32 ownerAvatar, const QString &userId, const QString &username, qint32 userAvatar)
    : ChatWindow(parent)
{
    this->ownerId = ownerId;
    this->ownername = ownername;
    this->ownerAvatar = ownerAvatar;
    this->userId = userId;
    this->username = username;
    this->userAvatar = userAvatar;

    ui->friendNameLabel->setText(username);
    setWindowTitle(QString("和%1的对话").arg(username));

}

void ChatWindow::onQmlSignal(QString msg) {
    qDebug() << "in slot: onQmlSignal";
//    ui->msgSendTextEdit->insertPlainText(msg);
    ChatMessage chatMessage(ownerId, userId, msg, QDateTime::currentDateTime());
    emit signalSendMessageButtonClickedToClient(Message::FromChatMessage(chatMessage));
    showSendMessage(msg);

}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::mousePressEvent(QMouseEvent *e)
{
  //鼠标左键
  if(e->button() == Qt::LeftButton)
  {
  m_ptPress = e->pos();
//  qDebug() << pos() << e->pos() << m_ptPress;
  m_bPressed = m_areaMovable.contains(m_ptPress);
  }
}

void ChatWindow::mouseMoveEvent(QMouseEvent *e)
{
  if(m_bPressed)
  {
//  qDebug() << pos() << e->pos() << m_ptPress;
  move(pos() + e->pos() - m_ptPress);
  }
}

void ChatWindow::mouseReleaseEvent(QMouseEvent *)
{
  m_bPressed = false;
}

//设置鼠标按下的区域
void ChatWindow::setAreaMovable(const QRect rt)
{
  if(m_areaMovable != rt)
  {
  m_areaMovable = rt;
  }
}

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

        qDebug() << QString("In msg: sendId %1 receivedid %2").arg(chatMsg.sendId).arg(chatMsg.receiveId);
    qDebug() << QString("In window: ownerId %1 userId %2").arg(ownerId).arg(userId);

        if(chatMsg.receiveId == userId && chatMsg.sendId == ownerId) {
//            ui->msgShowTextBrowser->append(QString("%1(我):").arg(chatMsg.sendId));
//            ui->msgShowTextBrowser->append(chatMsg.msg);
              showSendMessage(chatMsg.msg);
        } else if(chatMsg.sendId == userId && chatMsg.receiveId == ownerId) {
//            ui->msgShowTextBrowser->append(QString("%1(对方):").arg(chatMsg.sendId));
//            ui->msgShowTextBrowser->append(chatMsg.msg);
              showReceivedMessage(chatMsg);
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
    {
        ChatLogMessage chatLogMessage = Message::toChatLogMessage(dataStream);
        if(chatLogMessage.friendId != userId)	return;
        for(const ChatMessage &chatMsg: chatLogMessage.messageList) {
            if(chatMsg.receiveId == userId) {
                if(chatMsg.msg.contains("系统：")) continue;
//                ui->msgShowTextBrowser->append(QString("%1(我):").arg(chatMsg.sendId));
//                ui->msgShowTextBrowser->append(chatMsg.msg);
                showSendMessage(chatMsg.msg);
            } else if(chatMsg.sendId == userId) {
//                ui->msgShowTextBrowser->append(QString("%1(对方):").arg(chatMsg.sendId));
//                ui->msgShowTextBrowser->append(chatMsg.msg);
                showReceivedMessage(chatMsg);
            }
        }
        break;
    }
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
    case Message::FILE_MESSAEG:
    {
        FileMessage fileMessage = Message::toFileMessage(dataStream);
        if(fileMessage.receiveId != ownerId || fileMessage.sendId != userId) return;

        writeFileFromQByteArray(QDir::current().absolutePath(), fileMessage.fileName, fileMessage.fileContent, fileMessage.state);

//        ui->msgShowTextBrowser->append(QString("你收到了一个文件%1, 已存于%2").arg(fileMessage.fileName).arg(QDir::current().absolutePath()));
        showReceivedMessage(QString("系统：\n你收到了一个文件%1\n**单击打开**").arg(fileMessage.fileName), QDir::current().filePath(fileMessage.fileName));

        ChatMessage chatMessage(ownerId, userId, QString("系统：你发送的文件已被成功接受"), QDateTime::currentDateTime());
        emit signalSendMessageButtonClickedToClient(Message::FromChatMessage(chatMessage));
        break;
    }
    default:
        break;
    }
}

void ChatWindow::onCreateWindowButtonClickedFromClient()
{
    changeMode(modeFlag);
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
//    ui->msgShowTextBrowser->append(QString("%1(我)").arg(ownerId));
//    ui->msgShowTextBrowser->append(sendData);
    showSendMessage(sendData);

    QString sendId = ownerId, receiveId = this->userId;
    struct ChatMessage dataSrc(sendId, receiveId, sendData, QDateTime::currentDateTime());
    const QByteArray msg = Message::FromChatMessage(dataSrc);
    emit signalSendMessageButtonClickedToClient(msg);
}
QList<QByteArray> splitByteArray(QList<QByteArray> &chunks, const QByteArray &data, const qint64 chunkSize)
{
//    QList<QByteArray> chunks;
    int numChunks = (data.size() + chunkSize - 1) / chunkSize; // 计算需要的块数

    qDebug() << "in splitByteArray: " << data.size() << " " << chunkSize;
    for (int i = 0; i < numChunks; ++i)
    {
        QByteArray chunk = data.mid(i * chunkSize, chunkSize);
        chunks.append(chunk);
    }

    return chunks;
}
QByteArray readFileAsQByteArray(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file:" << filePath;
        return QByteArray();
    }
    return file.readAll();
}

void writeFileFromQByteArray(const QString &fileDir, const QString &fileName,  const QByteArray &data, const Message::FileState &state) {
    qDebug() << state;
    QDir directory(fileDir);
    QString filePath = directory.filePath(fileName);
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();
}


FileMessage readFileAsFileMessage(const QString &sendId, const QString &receiveId,
                                  const QString &filePath){
    qDebug() << "in readFileAsFileMessage";

    QFileInfo fileInfo(filePath);
    QByteArray content = readFileAsQByteArray(filePath);
    FileMessage fileMsg(sendId, receiveId, fileInfo.fileName(), content);
    return fileMsg;
}


void ChatWindow::on_sendFileButton_clicked()
{
    QString fileName =
    QFileDialog::getOpenFileName(
     this,QObject::tr("文件对话框"), //标题
     "C:", //初始路径
     QObject::tr("传输文件(*)"));
    qDebug() << "in slot: sendFile" << fileName;

    if(fileName.length() <= 0)  return;

    FileMessage fileMsg = readFileAsFileMessage(this->ownerId, this->userId, fileName);

     const QByteArray &msg = Message::FromFileMessage(fileMsg);
     emit signalSendMessageButtonClickedToClient(msg);

}

void ChatWindow::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time,  QNChatMessage::User_Type type)
{
    messageW->setFixedWidth(this->width());
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(text, time, size, type);
    ui->listWidget->setItemWidget(item, messageW);
}

void ChatWindow::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if(ui->listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->listWidget->item(ui->listWidget->count() - 1);
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
//        isShowTime = true;
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        QNChatMessage* messageTime = new QNChatMessage(ui->listWidget->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(ui->listWidget);

        QSize size = QSize(this->width(), 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setText(curMsgTime, curMsgTime, size, QNChatMessage::User_Time);
        ui->listWidget->setItemWidget(itemTime, messageTime);
    }
}

void ChatWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);


//    ui->msgSendTextEdit->resize(this->width() - 20, ui->sendBottom->height() - 20);
//    ui->msgSendTextEdit->move(10, 10);

//    ui->sendPushButton->move(ui->msgSendTextEdit->width()+ui->msgSendTextEdit->x() - ui->sendPushButton->width() - 10,
//                         ui->msgSendTextEdit->height()+ui->msgSendTextEdit->y() - ui->sendPushButton->height() - 10);


    for(int i = 0; i < ui->listWidget->count(); i++) {
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
        QListWidgetItem* item = ui->listWidget->item(i);

        dealMessage(messageW, item, messageW->text(), messageW->time(), messageW->userType());
    }
}

void ChatWindow::showSendMessage(const QString &msg) {
      qDebug() << "in showSendMessage";
        ui->msgSendTextEdit->setText("");
        QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳

        bool isSending = true; // 发送中

        qDebug()<<"addMessage" << msg << time << ui->listWidget->count();
//        if(ui->listWidget->count()%2) {
            if(isSending) {
                dealMessageTime(time);

                QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget(), ownerAvatar, userAvatar);
                QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
                dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
            } else {
                bool isOver = true;
                for(int i = ui->listWidget->count() - 1; i > 0; i--) {
                    QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
                    if(messageW->text() == msg) {
                        isOver = false;
                        messageW->setTextSuccess();
                    }
                }
                if(isOver) {
                    dealMessageTime(time);

                    QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget(), ownerAvatar, userAvatar);
                    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
                    dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
                    messageW->setTextSuccess();
                }
            }
//        } else {
//            if(msg != "") {
//                dealMessageTime(time);

//                QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget());
//                QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
//                dealMessage(messageW, item, msg, time, QNChatMessage::User_She);
//            }
//        }
        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}

void ChatWindow::showReceivedMessage(const ChatMessage &chatMessage) {
    if(chatMessage.msg != "") {
        QString time = QString::number(chatMessage.dateTime.toTime_t());
        dealMessageTime(time);

        QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget(), ownerAvatar, userAvatar);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        dealMessage(messageW, item, chatMessage.msg, time, QNChatMessage::User_She);
    }
}

void ChatWindow::showReceivedMessage(const QString &msg, const QString &fPath) {
    if(msg != "") {
        QString time = QString::number(QDateTime::currentDateTime().toTime_t());
        dealMessageTime(time);

        QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget(), ownerAvatar, userAvatar, fPath);
        connect(messageW, &QNChatMessage::messageRectClicked,this , &ChatWindow::handleMessageRectClicked);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        dealMessage(messageW, item, msg, time, QNChatMessage::User_She);
    }
}

void ChatWindow::on_sendPictureButton_clicked()
{
    QString fileName =
      QFileDialog::getOpenFileName(
       this,QObject::tr("文件对话框"), //标题
       "C:", //初始路径
       QObject::tr("图片文件(*png *jpg *jpeg *gif *bmp)"));

          qDebug()<< "in slot: sendPicture clicked" << fileName;

          if(fileName.length() <= 0)    return;

          FileMessage fileMsg = readFileAsFileMessage(this->ownerId, this->userId, fileName);

           const QByteArray &msg = Message::FromFileMessage(fileMsg);
           emit signalSendMessageButtonClickedToClient(msg);
}


void ChatWindow::on_emojiButton_clicked()
{
    delete engine;
    engine = new QQmlApplicationEngine();
    engine->rootContext()->setContextProperty("chatWindow", this);
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    connect(engine->rootObjects().first(), SIGNAL(qmlSignal(QString)), this, SLOT(onQmlSignal(QString)));
}

void ChatWindow::on_minimizeButton_clicked()
{
    showMinimized();
}

void ChatWindow::on_closeButton_clicked()
{
    close();
}

void ChatWindow::changeMode(const int modeId){
    qDebug() << "in f: changeMode";
    modeFlag = modeId;
    if(modeId){
        setStyleSheet("*{background-color: rgb(255, 255, 255);}"
                         "QScrollBar:vertical{"
                         "margin:16px 0px 16px 0px;"
                         "background-color:rgb(13, 129, 224);"
                         "border:0px;"
                         "width:14px;}"
                         "QScrollBar::handle:vertical{"
                         "background-color:rgba(144, 202, 249,190);"
                         "border-radius:7px;"
                         "width:13px;}"
                         "QScrollBar::handle:vertical:hover{"
                         "background-color:rgba(144, 202, 249,220);}"
                         "QScrollBar::sub-line:vertical{"
                         "subcontrol-position:top;"
                         "subcontrol-origin:margin;"
                         "background-color:rgb(13, 129, 224);"
                         "border:1px solid rgb(13, 129, 224);"
                         "height:16px;}"
                         "QScrollBar::add-line:vertical{"
                         "subcontrol-position:bottom;"
                         "subcontrol-origin:margin;"
                         "background-color:rgb(13, 129, 224);"
                         "border:1px solid rgb(13, 129, 224);"
                         "height:16px;}"
                         "QScrollBar::up-arrow:vertical{"
                         "border-image:url(:/commonImg/Resources/image/common/arrowUp.png);"
                         "width:12px;"
                         "height:6px;"
                         "}"
                         "QScrollBar::down-arrow:vertical{"
                         "border-image:url(:/commonImg/Resources/image/common/arrowDown.png); "
                         "width:12px;  "
                         "height:6px;}"
                         "QScrollBar::sub-page:vertical,QScrollBar::add-page:vertical{"
                         "background-color:rgb(13, 129, 224);"
                         "}");
        ui->listWidget->setStyleSheet("QListWidget{background-color: rgb(255, 255, 255); color:rgb(51,51,51); border: none;outline:0px;}"
                                      "QListWidget::Item{background-color: rgb(255, 255, 255);}"
                                      "QListWidget::Item:hover{background-color: rgb(255, 255, 255); }"
                                      "QListWidget::item:selected{"
                                          "background-color: rgb(255, 255, 255);"
                                          "color:black; "
                                          "border: 1px solid  rgb(255, 255, 255);"
                                      "}"
                                      "QListWidget::item:selected:!active{border: 1px solid  rgb(255, 255, 255); background-color: rgb(255, 255, 255); color:rgb(51,51,51); } ");
        ui->msgSend->setStyleSheet("background-color: rgb(240, 240, 240);") ;
        ui->friendName->setStyleSheet("background-color: rgb(36, 200, 254);");
    }
    else{
        setStyleSheet("*{background-color: rgb(64, 65, 66);}"
                         "QScrollBar:vertical{"
                         "margin:16px 0px 16px 0px;"
                         "background-color:rgb(11,54,117);"
                         "border:0px;"
                         "width:14px;}"
                         "QScrollBar::handle:vertical{"
                         "background-color:rgba(59,103,168,190);"
                         "border-radius:7px;"
                         "width:13px;}"
                         "QScrollBar::handle:vertical:hover{"
                         "background-color:rgba(59,103,168,220);}"
                         "QScrollBar::sub-line:vertical{"
                         "subcontrol-position:top;"
                         "subcontrol-origin:margin;"
                         "background-color:rgb(11,54,117);"
                         "border:1px solid rgb(11,54,117);"
                         "height:16px;}"
                         "QScrollBar::add-line:vertical{"
                         "subcontrol-position:bottom;"
                         "subcontrol-origin:margin;"
                         "background-color:rgb(11,54,117);"
                         "border:1px solid rgb(11,54,117);"
                         "height:16px;}"
                         "QScrollBar::up-arrow:vertical{"
                         "border-image:url(:/commonImg/Resources/image/common/arrowUp.png);"
                         "width:12px;"
                         "height:6px;"
                         "}"
                         "QScrollBar::down-arrow:vertical{"
                         "border-image:url(:/commonImg/Resources/image/common/arrowDown.png); "
                         "width:12px;  "
                         "height:6px;}"
                         "QScrollBar::sub-page:vertical,QScrollBar::add-page:vertical{"
                         "background-color:rgb(11,54,117);"
                         "}");
        ui->listWidget->setStyleSheet("QListWidget{background-color: rgb(64, 65, 66); color:rgb(51,51,51); border: none;outline:0px;}"
                                      "QListWidget::Item{background-color: rgb(64, 65, 66);}"
                                      "QListWidget::Item:hover{background-color: rgb(64, 65, 66); }"
                                      "QListWidget::item:selected{"
                                          "background-color: rgb(64, 65, 66);"
                                          "color:black; "
                                          "border: 2px solid  rgb(64, 65, 66);"
                                      "}"
                                      "QListWidget::item:selected:!active{border: 1px solid  rgb(64, 65, 66); background-color: rgb(64, 65, 66); color:rgb(51,51,51); } ");
      ui->msgSend->setStyleSheet("background-color: rgb(30, 30, 30);color:rgb(255,255,255);") ;
      ui->friendName->setStyleSheet("background-color:rgb(21, 120, 150);");
    }
}

void ChatWindow::handleMessageRectClicked(QString filePath) {

    QFileInfo fileInfo(filePath);
    qint64 fileSize = fileInfo.size();
//    qDebug()<<folderPath;
    SendFile *sendFileWidget = new SendFile(this);
    sendFileWidget->initsender(filePath, fileInfo, fileSize);
    sendFileWidget->show();
}
