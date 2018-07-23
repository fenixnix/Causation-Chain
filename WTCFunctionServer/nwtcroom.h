#ifndef NWTCROOM_H
#define NWTCROOM_H

#include "nwtcuser.h"

class NWTCRoom
{
public:
    NWTCRoom();
    void Add(NWTCUser user);
    int Size();
    bool IsExist(QString addr);
    void AssignRoomID();
    QStringList GetMembers();
    QString GetHash();
    QString Print();
private:
    QHash<QString, NWTCUser> members;
};

#endif // NWTCROOM_H
