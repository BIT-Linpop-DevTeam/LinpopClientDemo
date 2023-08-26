#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include "message.h"

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

private:
    Ui::ChatWindow *ui;
    QString ownerId, userId, username;

signals:
    void signalSendMessageButtonClickedToClient(const QByteArray &msg);

public slots:
    void onReadyReadFromClient(const QByteArray &msg);
    void onCreateWindowButtonClickedFromClient();
    void onSendMessageButtonClicked();
};

#endif // CHATWINDOW_H
