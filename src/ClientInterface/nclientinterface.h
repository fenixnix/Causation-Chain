#ifndef NCLIENTINTERFACE_H
#define NCLIENTINTERFACE_H

#include <QObject>
#include "udpnetwork.h"
#include "onnconnector.h"
#include "utility.h"
#include "ncryptomsg.h"
#include "ntimesync.h"
#include "nsimplestore.h"

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
    QString GetUrl();
    QString GetContract();
    void SendLocalMsg(QString cmd, QString msg);

    void StartTestTick();
    //Onn Test Code
    void JoinTank();
    void CloseTank();

signals:
    void RcvMsg(QString id, QString msg);
    void TrigOnnTick(int frame);

    void OnnInitSign(QByteArray secKey, QByteArray pubKey);
    void OnnJoinSign();
    void OnnCloseSign();
    void OnnPlaySign(QString dat);

public slots:
    void OnStartGame(QString jsonArrayMembers);
    void OnLoopTick(int frm);
    void OnTick(int frm);//Tick
    void OnRcvLocal(QString msg, QHostAddress senderIP, quint16 senderPort);

private slots:
    void OnGameTick(QString dat);
    void OnOnnTick(int frame, QString msg);

private:
    void RcvLocalCause(QString data);
    void RcvLocalResult(QString data);
    OnnConnector onn;
    NCryptoMsg crypto;

    UdpNetwork ipc;

    NTimeSync timeSync;

    NSimpleStore causeStore;
    NSimpleStore resultStore;
};

#endif // NCLIENTINTERFACE_H
