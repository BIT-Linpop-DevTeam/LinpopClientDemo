#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QByteArray>
#include "message.h"
#include "chatwindow.h"
#include "addfriend.h"
#include <QSet>
#include "userprofile.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

    void initClient();

    void update(const QString &userId, const QString &username, qint32 avatarId);

private:
    QString userId, username;
    qint32 avatarId;

private:
    /**
     * @brief addChat 创建与特定好友聊天的函数
     * @param ownerId
     * @param ownername
     * @param ownerAvatar
     * @param userId
     * @param username
     * @param userAvatar
     */
    void addChat(const QString &ownerId, const QString &ownername, const qint32 &ownerAvatar, const QString &userId, const QString &username, const qint32 &userAvatar);

private:
    Ui::Client *ui;
    QList<ChatWindow *> chatWindowList;
    addFriend *addFriendWindow;
    QSet<QString> idSet;
    UserProfile *userProfile;

signals:
    /**
     * @brief signalSendMessageToCommunicator 将要发送的消息转交给通信类进行发送
     * @param msg
     */
    void signalSendMessageToCommunicator(const QByteArray &msg);
    /**
     * @brief signalReadyReadToChat 将需要下发给聊天窗口的消息发给聊天窗口
     * @param msg
     */
    void signalReadyReadToChat(const QByteArray &msg);
    /**
     * @brief signalLoginCheckToLogin 转发登录验证结果给登录界面
     * @param msg
     */
    void signalLoginCheckToLogin(const QByteArray &msg);

public slots:
    /**
     * @brief onSendMessageButtonFromChat 转发聊天窗口发送的消息给通讯类
     * @param msg
     */
    void onSendMessageButtonFromChat(const QByteArray &msg);
    /**
     * @brief onSendMessageFromChildToCommunitor 转发下属界面需要发送的消息给通讯类
     * @param msg
     */
    void onSendMessageFromChildToCommunitor(const QByteArray &msg);

    /**
     * @brief onReadyReadFromCommunicator 收到来自通讯类的消息
     * @param msg
     */
    void onReadyReadFromCommunicator(const QByteArray &msg);
    /**
     * @brief onAddFriendButtonClicked 添加好友功能的槽函数
     */
    void onAddFriendButtonClicked();
    /**
     * @brief onConfirmUsernameFromChange 改变用户名
     * @param username
     */
    void onConfirmUsernameFromChange(const QString &username);
    /**
     * @brief onConfirmSignatureFromChange 改变签名
     * @param signature
     */
    void onConfirmSignatureFromChange(const QString &signature);
    /**
     * @brief onConfirmAvatarFromChange 改变头像
     * @param avatarId
     */
    void onConfirmAvatarFromChange(const qint32 &avatarId);

signals:
    /**
     * @brief closeLoginWindow
     */
    void closeLoginWindow();
    void updateFriendName(QString x);
public slots:
    void rcvLogin(QString userId, QString username, qint32 avatarId);
private slots:
    void on_closeButton_clicked();
    void on_minimizeButton_clicked();

    void on_modeBotton_clicked();

    void on_moreButton_clicked();

public:
    //设置鼠标按下可移动窗口的区域，在子窗口中必须设置该区域
    void setAreaMovable(const QRect rt);
    void switchPage();

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
