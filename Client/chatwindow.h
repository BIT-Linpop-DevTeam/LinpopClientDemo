#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include "message.h"
#include "qnchatmessage.h"
#include <QListWidgetItem>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <emojimodel.h>
#include <QApplication>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    explicit ChatWindow(QWidget *parent, const QString &ownerId, const QString &ownername, qint32 ownerAvatar, const QString &userId, const QString &username, qint32 userAvatar);
    ~ChatWindow();

    void setAreaMovable(const QRect rt);
    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
    void dealMessageTime(QString curMsgTime);

    void showSendMessage(const QString &msg);
    void showReceivedMessage(const ChatMessage &msg);
    void showReceivedMessage(const QString &msg);

    void changeMode(const int modeId);
protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::ChatWindow *ui;
    QString ownerId, ownername, userId, username;
    qint32 ownerAvatar, userAvatar;
    QQmlApplicationEngine *engine;

signals:
    void signalSendMessageButtonClickedToClient(const QByteArray &msg);

public slots:
    void onReadyReadFromClient(const QByteArray &msg);
    void onCreateWindowButtonClickedFromClient();
    void onSendMessageButtonClicked();
private slots:
    void on_sendFileButton_clicked();
    void on_sendPictureButton_clicked();
    void on_emojiButton_clicked();
    void onQmlSignal(QString msg);

    void on_minimizeButton_clicked();

    void on_closeButton_clicked();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    QRect m_areaMovable;//可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
    bool m_bPressed;//鼠标按下标志（不分左右键）
    QPoint m_ptPress;//鼠标按下的初始位置
};

#endif // CHATWINDOW_H
