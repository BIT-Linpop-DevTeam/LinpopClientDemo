#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "client.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();
signals:
    void userLogin(QString userId, QString username, qint32 avatarId);
    /**
     * @brief signalRequestLoginToCommunicator 要求通讯类转发登录验证的消息
     * @param msg
     */
    void signalRequestLoginToCommunicator(const QByteArray &msg);
    /**
     * @brief signalSignUpCheckMessageToRegister 要求通讯类转发注册验证的消息
     * @param msg
     */
    void signalSignUpCheckMessageToRegister(const QByteArray &msg);

public slots:
    /**
     * @brief onLoginCheckFromClient 收到服务器发来的登录验证消息
     * @param msg
     */
    void onLoginCheckFromClient(const QByteArray &msg);
    /**
     * @brief onRequestSignUpMessageFromRegister 收到注册界面发来的注册验证消息
     * @param msg
     */
    void onRequestSignUpMessageFromRegister(const QByteArray &msg);

public slots:
    void on_toolButton_2_clicked();

    void on_toolButton_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_editingFinished();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
    void loginWindowClose();
public:
    //设置鼠标按下可移动窗口的区域，在子窗口中必须设置该区域
    void setAreaMovable(const QRect rt);
    void initLogin(Client &client);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    Ui::Login *ui;
private:
    QRect m_areaMovable;//可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
    bool m_bPressed;//鼠标按下标志（不分左右键）
    QPoint m_ptPress;//鼠标按下的初始位置
};
#endif // LOGIN_H
