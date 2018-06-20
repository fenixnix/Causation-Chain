#ifndef CAUSATIONCONSENSUS_H
#define CAUSATIONCONSENSUS_H

#include <QObject>
#include <QList>
#include "ncausechain.h"
#include "nresultchain.h"
#include "nemcc.h"

class NCausationConsensus : public QObject
{
    Q_OBJECT
public:
    explicit NCausationConsensus(QObject *parent = nullptr);

signals:

public slots:
    void RcvCauseFromGame(QString data);
    void RcvResultFromGame(QString data, quint64 timeStamp);

    void RcvCauseFromCCN(QString sData);
    void RcvResultFromCCN(QString sData);

private:
    void SendCauseToGame(quint64 timeStamp, QString data);

    void BroadcastCauseToCCN(QString data);
    void BroadcastResultToCCN(QString data);

    void SendResultToGame(quint64 timeStamp,QString data);

    QString SignData(QString data, quint64 timeStamp);

    int timeStep = 100;//ms
    int netCapacity = 7;//totel member in net

    NEmcc emcc;
    NCauseChain causeChain;
    NResultChain resultChain;
};

#endif // CAUSATIONCONSENSUS_H
