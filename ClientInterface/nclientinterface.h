#ifndef NCLIENTINTERFACE_H
#define NCLIENTINTERFACE_H

#include <QObject>
#include "ncausationconsensus.h"
#include "ncmdpacker.h"
#include "np2pnode.h"
#include "ncryptomsg.h".h"
#include "np2pserverinterface.h"
#include "ntimesync.h"

#define StartPort 8890

class NClientInterface : public QObject
{
  Q_OBJECT
public:
  explicit NClientInterface(QObject *parent = nullptr);
  void Init();
  void SetPort(int port);
  QString GetLocalAddr();
  QStringList GetMemberList();
  void SendMsg(QString addr, QString msg);
  void StartTest();

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
  void OnCauseTimeOut();

private:
  void BroadcastCause();
  void RcvLocalCause(QString data);
  void RcvLocalResult(QString data);

  void RcvNetCause(quint64 frame, QString addr, QString data);
  void RcvNetCausePack(quint64 frame, QString addr, QString data);
  void RcvNetResult(quint64 frame, QString addr, QString data);

  QList<QString> articipators;

  QTimer timeOut;

  NP2PNode p2p;
  NCryptoMsg crypto;
  NP2PServerInterface ipc;

  NCmdPacker packer;
  NCausationConsensus consensus;//操作共识需要逐条共识，抛弃不确定操作
  NTimeSync timeSync;
};

#endif // NCLIENTINTERFACE_H
