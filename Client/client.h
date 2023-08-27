#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QByteArray>
#include "message.h"
#include "chatwindow.h"
#include "addfriend.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

    //todo
    void initClient();

    void update(const QString &userId, const QString &username);

private:
    QString userId, username;

private:
    void addChat(const QString &ownerId, const QString &userId, const QString &username);

private:
    Ui::Client *ui;
    QList<ChatWindow *> chatWindowList;
    addFriend *addFriendWindow;

signals:
    void signalSendMessageToCommunicator(const QByteArray &msg);
    void signalReadyReadToChat(const QByteArray &msg);
    void signalLoginCheckToLogin(const QByteArray &msg);

public slots:
    void onSendMessageButtonFromChat(const QByteArray &msg);
    void onSendMessageFromChildToCommunitor(const QByteArray &msg);

    //todo
    void onReadyReadFromCommunicator(const QByteArray &msg);
    void onAddFriendButtonClicked();

signals:
    void closeLoginWindow();
    void updateFriendName(QString x);
public slots:
    void rcvLogin(QString userId, QString username);
private slots:
    void on_closeButton_clicked();
    void on_minimizeButton_clicked();

public:
    //设置鼠标按下可移动窗口的区域，在子窗口中必须设置该区域
    void setAreaMovable(const QRect rt);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    QRect m_areaMovable;//可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
    bool m_bPressed;//鼠标按下标志（不分左右键）
    QPoint m_ptPress;//鼠标按下的初始位置
};
#endif // CLIENT_H
