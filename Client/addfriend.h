#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>

namespace Ui {
class addFriend;
}

class addFriend : public QWidget
{
    Q_OBJECT

public:
    explicit addFriend(QWidget *parent, const QString &userId, const QString &username);
    ~addFriend();
    void initAddFriend();

private:
    Ui::addFriend *ui;
    QString userId, username;

public slots:
    void onConfirmButtonClicked();
    void onCancelButtonClicked();
signals:
    void signalRequestFriendToClient(const QByteArray &msg);
};

#endif // ADDFRIEND_H
