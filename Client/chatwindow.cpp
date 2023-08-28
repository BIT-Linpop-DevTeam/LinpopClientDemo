#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#define BLOCK_SIZE 20
void writeFileFromQByteArray(const QString &fileDir, const QString &fileName,  const QByteArray &data, const Message::FileState &fileState);

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
        if(fileMessage.receiveId != ownerId || fileMessage.sendId != userId) return;

        writeFileFromQByteArray(QDir::current().absolutePath(), fileMessage.fileName, fileMessage.fileContent, fileMessage.state);

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
    QDir directory(fileDir);
    QString filePath = directory.filePath(fileName);
    QFile file(filePath);
    if(state == Message::FIRST){
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Failed to open file for writing:" << filePath;
            return;
        }
    }
    else if (!file.open(QIODevice::Append)) {
        qDebug() << "Failed to open file for writing:" << filePath;
        return;
    }

    qint64 bytesWritten = file.write(data);
    if (bytesWritten == -1) {
        qDebug() << "Failed to write data to file:" << filePath;
        return;
    }

    qDebug() << "Written" << bytesWritten << "bytes to file:" << filePath;
//    if(state == Message::LAST)
    file.close();

}

void readFileAsFileMessage(QList<FileMessage> &fileMsgList, const QString &sendId, const QString &receiveId,
                                  const QString &filePath){
    qDebug() << "in readFileAsFileMessage";

    QFileInfo fileInfo(filePath);
    QByteArray content = readFileAsQByteArray(filePath);
    QList<QByteArray> byteList;
    splitByteArray(byteList, content, BLOCK_SIZE);
    qDebug() << "when read file, size of bytes block";
    FileMessage fileMsg;
    for (int i = 0; i < byteList.size(); i++){
        fileMsg.sendId = sendId, fileMsg.receiveId = receiveId, fileMsg.fileName = fileInfo.fileName();
        fileMsg.fileContent = byteList[i];
        if(!i) fileMsg.state = Message::FIRST;
        else if(i == byteList.size() - 1) fileMsg.state = Message::LAST;
        else fileMsg.state = Message::MID;
        fileMsgList.append(fileMsg);
    }
    qDebug() << "after read file, size of list: " << fileMsgList.size();
}


void ChatWindow::on_sendFileButton_clicked()
{
    QString fileName =
    QFileDialog::getOpenFileName(
     this,QObject::tr("文件对话框"), //标题
     "C:", //初始路径
     QObject::tr("传输文件(*)"));
    qDebug() << "in slot: sendFile" << fileName;

    QList<FileMessage> fileMsgList;
    readFileAsFileMessage(fileMsgList, this->ownerId, this->userId, fileName);
    qDebug() << "in slot: send File button clicked: list size, " <<fileMsgList.size();
    for (int i = 0; i < fileMsgList.size(); i++){
        qDebug() << "state: " << fileMsgList.at(i).state;
        const QByteArray &msg = Message::FromFileMessage(fileMsgList[i]);
        emit signalSendMessageButtonClickedToClient(msg);
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

      QList<FileMessage> fileMsgList;
      readFileAsFileMessage(fileMsgList, this->ownerId, this->userId, fileName);
      for (int i = 0; i < fileMsgList.size(); i++){
          const QByteArray &msg = Message::FromFileMessage(fileMsgList.at(i));
          emit signalSendMessageButtonClickedToClient(msg);
      }
}
