#ifndef NCLIENTINTERFACE_H
#define NCLIENTINTERFACE_H

#include <QObject>
#include <QTimer>
//#include <QProcess>
#include "udpnetwork.h"
#include "utility.h"
//#include "ntimesync.h"
#include "nsimplestore.h"
#include "onnconnector.h"

#define StartPort 8890
#define ONN

class NClientInterface : public QObject
{
    Q_OBJECT
public:
    explicit NClientInterface(QObject *parent = nullptr);
    ~NClientInterface();
    void Init();
    void Init(QString secKey, QString pubKey);
    void Close();
    QString GetID();
    QString GetUrl();
    QString GetContract();
    void SendLocalMsg(QString cmd, QString msg);

    void StartTestTick();
    //Onn Test Code
    void JoinTank(QString jsonArgs);
    void CloseTank();
    void LoadTankReplay(QString fileName);
    void StartTank();

signals:
    void RcvMsg(QString id, QString msg);
    void TrigOnnTick(int frame);

    void OnnInitSign(QByteArray secKey, QByteArray pubKey);
    void OnnJoinSign(QString jsonArgs);
    void OnnCloseSign();
    void OnnPlaySign(QString dat);

public slots:
    void OnStartGame(QString jsonArrayMembers);
    void OnLoopTick(int frm);
    void OnTick(int frm);//Tick
    void OnRcvLocal(QString msg, QHostAddress senderIP, quint16 senderPort);
    void OnLoad();

private slots:
    void OnGameTick(QString dat);
    void OnOnnTick(int frame, QString msg);

private:
    void RcvLocalCause(QString data);
    void RcvLocalResult(QString data);
    void RcvLocalOpr(QString data);

#ifdef ONN
    OnnConnector onn;
#endif
    QString localNickName;
    NCryptoMsg crypto;
    UdpNetwork ipc;
    NTimeSync timeSync;
    StopWatch sw;

    NSimpleStore causeStore;
    NSimpleStore resultStore;
    QTimer loadTimer;
    //QProcess process;
    void InitDataStorage();
};

#endif // NCLIENTINTERFACE_H
