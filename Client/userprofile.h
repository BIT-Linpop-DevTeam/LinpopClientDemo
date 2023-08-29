//UserProfile.h
#ifndef UserProfile_H
#define UserProfile_H

#include <QWidget>
#include <QListWidgetItem>

class QStackedWidget;

namespace Ui {
class UserProfile;
}

class UserProfile : public QWidget
{
    Q_OBJECT

public:
    explicit UserProfile(QWidget *parent = 0);
    ~UserProfile();
    void switchPage();//切换函数

signals:
    void signalConfirmUsernameToClient(const QString &username);
    void signalConfirmSignatureToClient(const QString &username);
    void signalConfirmAvatarToClient(const qint32 &avatarId);

private slots:
    void on_toolButton_2_clicked();

    void on_toolButton_clicked();

    void on_cancelButton_2_clicked();

    void on_cancelButton_3_clicked();

    void on_cancelButton_1_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void onConfirmUsernameClicked();
    void onConfirmSignatureClicked();
    void onConfirmAvatarClicked();

private:
    Ui::UserProfile *ui;
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

#endif // UserProfile_H
