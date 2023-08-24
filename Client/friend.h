#ifndef FRIEND_H
#define FRIEND_H
#include <QString>

class Friend
{
public:
    explicit Friend(const QString &FriendID, const QString &FriendName);
private:
    QString FriendId;
    QString FriendName;
};

#endif // FRIEND_H
