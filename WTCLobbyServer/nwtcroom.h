#ifndef NWTCROOM_H
#define NWTCROOM_H

#include <QObject>
#include "nwtcuser.h"
#include <QTimer>
#include "ntcpnetwork.h"

class NWTCRoom : public QObject
{
    Q_OBJECT
public:
    explicit NWTCRoom(QObject *parent = nullptr);
    void Add(NWTCUser user);
    int Size();
    bool IsExist(QString addr);
    void AssignRoomID();
    void Start();
    void Close();//???
    QStringList GetMembers();
    QString GetHash();
    QString Print();

    QHash<QString, NWTCUser> members;
    QTimer timer;
    QString roomID;

private slots:
    void OnTrig();

private:

};

#endif // NWTCROOM_H
