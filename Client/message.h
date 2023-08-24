#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <QString>

class Message
{
public:
    explicit Message(const QByteArray& dataSrc);
    explicit Message(const QString &dataSrc = nullptr);
    QByteArray toByteArray() const;
    QString toString() const;
private:
    QString msg;
};

#endif // MESSAGE_H
