#ifndef ACCEPTFRIEND_H
#define ACCEPTFRIEND_H

#include <QWidget>

namespace Ui {
class Acceptfriend;
}

class Acceptfriend : public QWidget
{
    Q_OBJECT

public:
    explicit Acceptfriend(QWidget *parent, const QString &userId, const QString &friendId);
    ~Acceptfriend();

private:
    Ui::Acceptfriend *ui;
    QString userId, friendId;

signals:
    void signalRequestFriendMessageToClient(const QByteArray &msg);
public slots:
    void onConfirmButtonClicked();
    void onCancelButtonClicked();
};

#endif // ACCEPTFRIEND_H
