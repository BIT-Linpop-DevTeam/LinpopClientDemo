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
    ~ChatWindow();

private:
    Ui::ChatWindow *ui;

signals:
    void signalSendMessageButtonClickedToClient(const Message &msg);

public slots:
    void onReadyReadFromClient(const Message &msg);
    void onCreateWindowButtonClickedFromClient();
    void onSendMessageButtonClicked();
};

#endif // CHATWINDOW_H
