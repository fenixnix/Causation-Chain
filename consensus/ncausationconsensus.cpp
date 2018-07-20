#include "ncausationconsensus.h"
#include <QDateTime>
#include <QCryptographicHash>

NCausationConsensus::NCausationConsensus(QObject *parent) : QObject(parent)
{

}

void NCausationConsensus::RcvCause(quint64 frame, QString id, QString data)
{
    causeChain.Add(NConsensusMetadata(frame, id, data));
    if(causeChain.consensusSize(frame)>=netCapacity){
        emit ReachCauseConsensus(frame, causeChain.consensusData(frame));
    }
}

void NCausationConsensus::RcvResult(quint64 frame, QString id, QString data)
{
    resultChain.Add(NConsensusMetadata(frame, id, data));
    if(resultChain.consensusSize(frame)>=netCapacity){
        emit ReachResultConsensus(frame, resultChain.consensusData(frame));
    }
}

void NCausationConsensus::RcvResultHash(quint64 frame, QString id, QByteArray hash)
{
    resultChain.Add(NConsensusMetadata(frame, id, hash));
    if(resultChain.consensusSize(frame)>=netCapacity){
        emit ReachResultConsensus(frame, resultChain.consensusData(frame));
    }
}

QString NCausationConsensus::SelfTest()
{
    QString txt;
    QTextStream ts(&txt);
    ts<<NConsensusChain::SelfTest();
    return txt;
}
