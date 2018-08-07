#ifndef NCLIENTINTERFACE_H
#define NCLIENTINTERFACE_H

#include <QObject>
#include "ncausationconsensus.h"
#include "ncmdpacker.h"
#include "np2pnode.h"
#include "ncryptomsg.h"
#include "np2pserverinterface.h"
#include "udpnetwork.h"
#include "ntimesync.h"
#include "onnconnector.h"
#include "ntcpnetwork.h"

#define StartPort 8890

class NClientInterface : public QObject
{
    Q_OBJECT
public:
    explicit NClientInterface(QObject *parent = nullptr);
    void Init();
    void Init(QString secKey, QString pubKey);
    void SetPort(int port);
    QString getID();
    QStringList GetMemberList();
    void SetLocalID(QString ID);
    void SendChatMsg(QString addr, QString msg);
    void SendLocalMsg(QString cmd, QString msg);

    //To Lobby
    void Enter_Lobby();//local nat
    void Queue_Solo();//local addr
    void Game_Over(QString Result);//local game result


    void SendGameInitInfo(QString data);

    //Test Code
    void StartTest();
    void EnterLobby();
    void StartSoloQueue();
    void JoinTank();

signals:
    void RcvCause(QString cause);
    void RcvResult(QString result);
    void RcvMsg(QString id, QString msg);
    //void RcvResultHash(QByteArray resultHash);

public slots:
    void OnInit(QString msg);//功能业务初始化 业务Server
    void OnFinish(QString msg);//功能业务结束 业务Server
    void OnTick(int frameNo);

    void OnRcvLocal(QString msg, QHostAddress senderIP, quint16 senderPort);
    void OnRcvNet(quint64 timeStamp, QString addr, QString msg);

private slots:
    //Fram Lobby
    void OnLobbyNet(QString dat);
    void OnGameNet(QString dat);
    void OnGameStart(QString dat);
    void OnGameTick(QString dat);

    void OnCauseTimeOut();
    void OnRcvP2P(QString msg);
    void OnRcvServerMsg(QString cmd, QString msg);

    void OnReadyJoin();
    void OnOnnMsg(QString msg);

private:
    void BroadcastCause();
    void RcvStart(QString data);
    void RcvLocalCause(QString data);
    void RcvLocalResult(QString data);

    void RcvNetCause(quint64 frame, QString addr, QString data);
    void RcvNetCausePack(quint64 frame, QString addr, QString data);
    void RcvNetResult(quint64 frame, QString addr, QString data);

    void CryptoSend(QString id, QString msg);
    void CryptoBroadcast(QString msg);

    QList<QString> articipators;
    QMap<QString, QString> neighbourKeyMap;

    QTimer timeOut;

    NCryptoMsg crypto;
    NP2PNode p2p;
    NP2PServerInterface server;
    NTcpNetwork tcpServer;

    UdpNetwork ipc;

    OnnConnector onn;

    NCmdPacker packer;
    NCausationConsensus consensus;//操作共识需要逐条共识，抛弃不确定操作
    NTimeSync timeSync;
};

#endif // NCLIENTINTERFACE_H
