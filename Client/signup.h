#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include "login.h"

namespace Ui {
class SignUp;
}

class SignUp : public QWidget
{
    Q_OBJECT

public:
    SignUp(QWidget *parent = nullptr);

public:
    void setAreaMovable(const QRect rt);
    ~SignUp(){}

    void initSignUp(const Login &login);
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);


private slots:
    void on_toolButton_2_clicked();

    void on_toolButton_clicked();

    void on_pushButton_clicked();

private:
    QRect m_areaMovable;//可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
    bool m_bPressed;//鼠标按下标志（不分左右键）
    QPoint m_ptPress;//鼠标按下的初始位置
private:
    Ui::SignUp *ui;

signals:
    void signalRequestSignUpMessageToLogin(const QByteArray &msg);
public slots:
    void onSignUpCheckMessageFromLogin(const QByteArray &msg);
};

#endif // SIGNUP_H
