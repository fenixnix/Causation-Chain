#ifndef NWITNESSNETWORK_H
#define NWITNESSNETWORK_H

#include <QObject>
#include "ntcpnetwork.h"
#include "np2pserverinterface.h"
#include "np2pnode.h"
#include "ncryptomsg.h"
#include "ncausepacker.h"
#include "ncausationconsensus.h"
#include "ntimesync.h"

class NWitnessNetwork : public QObject
{
    Q_OBJECT
public:
    explicit NWitnessNetwork(QObject *parent = nullptr);
    void Init(QString jsonString);
    void Init(QJsonArray json);

    QString GetConsensusCause(int frame);//frame 0 = init then send to WTC
    void SetLocalCause(int frame, QString cause);//startwith frame 1
    void Sync(int frame);//finish rcv cause and broadcast cause pack;

private slots:
    void OnRcvMsg(QString msg);

private:
    NP2PNode p2p;
    NCausePacker packer;
    NCausationConsensus consensus;//操作共识需要逐条共识，抛弃不确定操作
    //for consensus
    QMap<QString, QString> articipatorsKeyMap;

    NCryptoMsg crypto;//for crypto
    NTimeSync timeSync;//for time Sync algr
};

#endif // NWITNESSNETWORK_H
