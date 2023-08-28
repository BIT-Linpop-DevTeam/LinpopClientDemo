#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
void writeFileFromQByteArray(const QString &fileDir, const QString &fileName,  const QByteArray &data);

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

        qDebug() << QString("In msg: sendId %1 receivedid %2").arg(chatMsg.sendId).arg(chatMsg.receiveId);
    qDebug() << QString("In window: ownerId %1 userId %2").arg(ownerId).arg(userId);
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
    {
        ChatLogMessage chatLogMessage = Message::toChatLogMessage(dataStream);
        if(chatLogMessage.friendId != userId)	return;
        for(const ChatMessage &chatMsg: chatLogMessage.messageList) {
            if(chatMsg.receiveId == userId) {
                ui->msgShowTextBrowser->append(QString("%1(我):").arg(chatMsg.sendId));
                ui->msgShowTextBrowser->append(chatMsg.msg);
            } else if(chatMsg.sendId == userId) {
                ui->msgShowTextBrowser->append(QString("%1(对方):").arg(chatMsg.sendId));
                ui->msgShowTextBrowser->append(chatMsg.msg);
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
        if(fileMessage.receiveId != userId) return;

        writeFileFromQByteArray(QDir::current().absolutePath(), fileMessage.fileName, fileMessage.fileContent);

        ui->msgShowTextBrowser->append(QString("你收到了一个文件%1, 已存于%2").arg(fileMessage.fileName).arg(QDir::current().absolutePath()));

        break;
    }
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
    ui->msgShowTextBrowser->append(QString("%1(我)").arg(ownerId));
    ui->msgShowTextBrowser->append(sendData);

    QString sendId = ownerId, receiveId = this->userId;
    struct ChatMessage dataSrc(sendId, receiveId, sendData, QDateTime::currentDateTime());
    const QByteArray msg = Message::FromChatMessage(dataSrc);
    emit signalSendMessageButtonClickedToClient(msg);
}

QByteArray readFileAsQByteArray(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file:" << filePath;
        return QByteArray();
    }
    return file.readAll();
}

void writeFileFromQByteArray(const QString &fileDir, const QString &fileName,  const QByteArray &data) {
    QDir directory(fileDir);
    QString filePath = directory.filePath(fileName);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing:" << filePath;
        return;
    }

    qint64 bytesWritten = file.write(data);
    if (bytesWritten == -1) {
        qDebug() << "Failed to write data to file:" << filePath;
        return;
    }

    qDebug() << "Written" << bytesWritten << "bytes to file:" << filePath;
    file.close();

}

FileMessage readFileAsFileMessage(const QString &sendId, const QString &receiveId,
                                  const QString &filePath){
    FileMessage fileMsg;
    QFileInfo fileInfo(filePath);
    fileMsg.sendId = sendId, fileMsg.receiveId = receiveId, fileMsg.fileName = fileInfo.fileName();
    fileMsg.fileContent = readFileAsQByteArray(filePath);
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

    FileMessage fileMessage = readFileAsFileMessage(this->ownerId, this->userId, fileName);
    const QByteArray &msg = Message::FromFileMessage(fileMessage);
    emit signalSendMessageButtonClickedToClient(msg);

}

void ChatWindow::on_sendPictureButton_clicked()
{
    QString fileName =
      QFileDialog::getOpenFileName(
       this,QObject::tr("文件对话框"), //标题
       "C:", //初始路径
       QObject::tr("图片文件(*png *jpg *jpeg *gif *bmp)"));

          qDebug()<< "in slot: sendPicture clicked" << fileName;

    FileMessage fileMessage = readFileAsFileMessage(this->ownerId, this->userId, fileName);
    const QByteArray &msg = Message::FromFileMessage(fileMessage);
    emit signalSendMessageButtonClickedToClient(msg);
}
