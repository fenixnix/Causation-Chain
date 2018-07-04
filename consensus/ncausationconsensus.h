#ifndef CAUSATIONCONSENSUS_H
#define CAUSATIONCONSENSUS_H

#include <QObject>
#include <QList>
#include <QtNetwork>
#include "ncausechain.h"
#include "nresultchain.h"
#include "nemcc.h"
#include "nclientinterface.h"

class NCausationConsensus : public QObject
{
    Q_OBJECT
public:
    explicit NCausationConsensus(QObject *parent = nullptr);

signals:

public slots:
    void RcvCauseFromUserClient(QString data);
    void RcvResultFromUserClient(QString data);

    void RcvCauseFromCCN(QString sData);
    void RcvResultFromCCN(QString sData);

private:
    void SendCauseToUserClient(quint64 timeStamp, QString data);
    void SendResultToUserClient(quint64 timeStamp,QString data);

    void BroadcastCauseToCCN(QString data);
    void BroadcastResultToCCN(QString data);

    QString SignData(QString data, quint64 timeStamp);

    int timeStep = 100;//ms
    int netCapacity = 7;//totel member in net

    NEmcc emcc;
    NCauseChain causeChain;
    NResultChain resultChain;
    NClientInterface interface;
};

#endif // CAUSATIONCONSENSUS_H
