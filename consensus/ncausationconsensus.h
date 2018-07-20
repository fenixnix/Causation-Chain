#ifndef CAUSATIONCONSENSUS_H
#define CAUSATIONCONSENSUS_H

#include <QObject>
#include <QList>
#include <QtNetwork>
#include "nconsensuschain.h"

class NCausationConsensus : public QObject
{
    Q_OBJECT
public:
    explicit NCausationConsensus(QObject *parent = nullptr);

signals:
    void ReachCauseConsensus(quint64 frame, QString cauData);
    void ReachResultConsensus(quint64 frame, QString resData);

public slots:
    void RcvCause(quint64 frame, QString id, QString data);
    void RcvResult(quint64 frame, QString id, QString data);
    void RcvResultHash(quint64 frame, QString id, QByteArray hash);
    static QString SelfTest();

private:
    void SendCauseToLocal(quint64 timeStamp, QString data);

    int timeStep = 100;//ms
    int netCapacity = 3;//totel member in net

    NConsensusChain causeChain;
    NConsensusChain resultChain;
};

#endif // CAUSATIONCONSENSUS_H
