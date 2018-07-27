#ifndef NWTCROOM_H
#define NWTCROOM_H

#include <QObject>
#include "nwtcuser.h"
#include <QTimer>
#include "udpnetwork.h"

class NWTCRoom : public QObject
{
    Q_OBJECT
public:
    explicit NWTCRoom(QObject *parent = nullptr);
    void Add(NWTCUser user);
    int Size();
    bool IsExist(QString addr);
    void AssignRoomID();
    void Start(UdpNetwork* udp);
    void Close();//???
    QStringList GetMembers();
    QString GetHash();
    QString Print();

private slots:
    void OnTrig();

private:
    QHash<QString, NWTCUser> members;
    QTimer timer;
    UdpNetwork* udp;
    QString roomID;
};

#endif // NWTCROOM_H
