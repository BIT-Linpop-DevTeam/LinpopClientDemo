#include "message.h"

Message::Message(const QByteArray &dataSrc)
{
    msg = QString::fromUtf8(dataSrc);
}

Message::Message(const QString &dataSrc)
{
    msg = dataSrc;
}

QByteArray Message::toByteArray() const {
    return this->msg.toUtf8();
}

QString Message::toString() const {
    return this->msg;
}

