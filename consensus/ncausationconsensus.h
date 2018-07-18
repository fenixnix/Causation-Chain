#ifndef CAUSATIONCONSENSUS_H
#define CAUSATIONCONSENSUS_H

#include <QObject>
#include <QList>
#include <QtNetwork>
#include "ncausechain.h"
#include "nresultchain.h"

class NCausationConsensus : public QObject
{
    Q_OBJECT
public:
    explicit NCausationConsensus(QObject *parent = nullptr);

signals:
    void ReachConsensus(quint64 timeStamp, QString resData);

public slots:
    void RcvCause(quint64 timeStamp, QString id, QString data);
    //void RcvCauseFromLocal(QString id, QString data);
    //void RcvCauseFromCCN(QString id, QString sData);
    void RcvResult(quint64 timeStamp, QString id, QString data);
    void RcvResultHash(quint64 timeStamp, QString id, QByteArray hash);
    //void RcvResultFromLocal(QString data);
    //void RcvResultFromCCN(QString sData);

    static QString SelfTest();

private:
    void SendCauseToLocal(quint64 timeStamp, QString data);
    //void SendResultToLocal(quint64 timeStamp,QString data);
    //void BroadcastCauseToCCN(QString data);
    //void BroadcastResultToCCN(QString data);
    //QString SignData(QString data, quint64 timeStamp);

    int timeStep = 100;//ms
    int netCapacity = 3;//totel member in net

    NCauseChain causeChain;
    NResultChain resultChain;
};

#endif // CAUSATIONCONSENSUS_H
