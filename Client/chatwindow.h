#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include "message.h"
#include "qnchatmessage.h"
#include <QListWidgetItem>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    explicit ChatWindow(QWidget *parent, const QString &ownerId, const QString &userId, const QString &username);
    ~ChatWindow();

    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
    void dealMessageTime(QString curMsgTime);

    void showSendMessage(const QString &msg);
    void showReceivedMessage(const QString &msg);
protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::ChatWindow *ui;
    QString ownerId, userId, username;

signals:
    void signalSendMessageButtonClickedToClient(const QByteArray &msg);

public slots:
    void onReadyReadFromClient(const QByteArray &msg);
    void onCreateWindowButtonClickedFromClient();
    void onSendMessageButtonClicked();
private slots:
    void on_sendFileButton_clicked();
    void on_sendPictureButton_clicked();
};

#endif // CHATWINDOW_H
