#ifndef NCLIENTINTERFACE_H
#define NCLIENTINTERFACE_H

#include <QObject>

#include "ntcpnetwork.h"
#include "ncausationconsensus.h"
#include "np2pserverinterface.h"
#include "ncausepacker.h"
#include "np2pnode.h"
#include "ncryptomsg.h"
#include "ntimesync.h"

#include "udpnetwork.h"
#include "onnconnector.h"
#include "utility.h"

#define StartPort 8890

class NClientInterface : public QObject
{
    Q_OBJECT
public:
    explicit NClientInterface(QObject *parent = nullptr);
    ~NClientInterface();
    void Init();
    void Init(QString secKey, QString pubKey);
    QString getID();
    QStringList GetMemberList();
    void SetLocalID(QString ID);
    void SendChatMsg(QString addr, QString msg);
    void SendLocalMsg(QString cmd, QString msg);

    //To Lobby
    void Enter_Lobby();//local nat
    void Queue_Solo();//local addr
    void Game_Over(QString Result);//local game result
    //Onn Test Code
    void JoinTank();
    void OnnInputs(int frame, QString msg);

signals:
    void RcvCause(QString cause);
    void RcvResult(QString result);
    void RcvMsg(QString id, QString msg);
    void TrigOnnTick(int frame);

    void OnnInitSign(QByteArray secKey, QByteArray pubKey);
    void OnnJoinSign();
    void OnnPlaySign(QString dat);

public slots:
    void OnStartGame(QString jsonArrayMembers);
    void OnTick(int frameNo);//Tick

    void OnRcvLocal(QString msg, QHostAddress senderIP, quint16 senderPort);
    void OnRcvNet(quint64 timeStamp, QString addr, QString msg);

private slots:
    //From Lobby
    void OnSubNet(QString dat);
    void OnGameTick(QString dat);

    void OnRcvP2P(QString msg);
    void OnRcvServerMsg(QString msg);
    void OnRcvServerCmdMsg(QString cmd, QString msg);

    void OnReadyJoin();

    void OnOnnTimer();
    void OnOnnTick(int frame, QString msg);

private:
    void SendGameInitInfo(QString data);

    void BroadcastCause();
    void RcvStart(QString data);
    void RcvLocalCause(QString data);
    void RcvLocalResult(QString data);

    void RcvNetCause(quint64 frm, QString addr, QString data);
    void RcvNetCausePack(quint64 frame, QString addr, QString data);
    void RcvNetResult(quint64 frame, QString addr, QString data);

    void CryptoSend(QString id, QString msg);
    void CryptoBroadcast(QString msg);

    OnnConnector onn;
    QTimer onnTimer;
    int onnFrame = 1;

    NP2PNode p2p;
    NTcpNetwork tcpServer;
    UdpNetwork ipc;

    NCausePacker packer;
    NCausationConsensus consensus;//操作共识需要逐条共识，抛弃不确定操作
    NTimeSync timeSync;
    //for consensus
    NCryptoMsg crypto;
    QList<QString> articipators;
    QMap<QString, QString> neighbourKeyMap;
};

#endif // NCLIENTINTERFACE_H
