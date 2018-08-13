#include "ncausationconsensus.h"
#include <QDateTime>
#include <QCryptographicHash>

NCausationConsensus::NCausationConsensus(QObject *parent) : QObject(parent)
{

}

void NCausationConsensus::RcvCause(quint64 frame, QString id, QString data)
{
    causeChain.Add(NConsensusMetadata(frame, id, data));
    if(CauseIsConsensus(frame)){
        emit ReachCauseConsensus(frame, CauseConsensus(frame));
    }
}

void NCausationConsensus::RcvResult(quint64 frame, QString id, QString data)
{
    resultChain.Add(NConsensusMetadata(frame, id, data));
    if(ResultIsConsensus(frame)){
        emit ReachResultConsensus(frame, resultChain.consensusData(frame));
    }
}

void NCausationConsensus::RcvResultHash(quint64 frame, QString id, QString data, QByteArray hash)
{
    resultChain.Add(NConsensusMetadata(frame, id, data, hash));
    if(ResultIsConsensus(frame)){
        emit ReachResultConsensus(frame, ResultHashConsensus(frame));
    }
}

bool NCausationConsensus::CauseIsConsensus(quint64 frame)
{
    return causeChain.consensusSize(frame) >= netCapacity;
}

bool NCausationConsensus::ResultIsConsensus(quint64 frame)
{
    return resultChain.consensusSize(frame)>= netCapacity;
}

QString NCausationConsensus::CauseConsensus(quint64 frame)
{
    return causeChain.consensusData(frame);
}

QByteArray NCausationConsensus::ResultHashConsensus(quint64 frame)
{
    return resultChain.consensusHash(frame);
}

QString NCausationConsensus::SelfTest()
{
    QString txt;
    QTextStream ts(&txt);
    ts<<NConsensusChain::SelfTest();
    return txt;
}
