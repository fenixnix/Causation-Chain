#include "ncausationconsensus.h"
#include <QDateTime>
#include <QCryptographicHash>

NCausationConsensus::NCausationConsensus(QObject *parent) : QObject(parent)
{

}

void NCausationConsensus::RcvCauseFromGame(QString data)
{
  quint64 timeStamp = QDateTime::currentMSecsSinceEpoch();
  SendCauseToGame(timeStamp, data);
  QString msg = SignData(data,timeStamp);
  BroadcastCauseToCCN(msg);
}

void NCausationConsensus::RcvResultFromGame(QString data,quint64 timeStamp)
{
  QString msg = SignData(data,timeStamp);
  BroadcastCauseToCCN(msg);
}

void NCausationConsensus::RcvCauseFromCCN(QString sData)
{
  auto sDatas = sData.split("+");
  if(sDatas.size()<4){
      qDebug()<<"Error Cause!!!";
      return;
    }
  QString rawData = sDatas[0] + "+" + sDatas[1] + "+" + sDatas[2];
  if(!emcc.VerifyMsg(sDatas[1],rawData,sDatas[3])){
      qDebug()<<"Cheat Data!!!";
      return;
    }
  NCause cause;
  cause.timeStamp = sDatas[0].toInt();
  cause.pubKey = sDatas[1];
  cause.id = emcc.calcAddress(sDatas[1]);
  cause.data = sDatas[2];
  causeChain.Add(cause.timeStamp, cause);

  SendCauseToGame(cause.timeStamp, cause.data);
}

void NCausationConsensus::SendCauseToGame(quint64 timeStamp, QString data)
{
  //TODO: UDP2Game
}

void NCausationConsensus::RcvResultFromCCN(QString sData)
{
  auto sDatas = sData.split("+");
  if(sDatas.size()<4){
      qDebug()<<"Error Cause!!!";
      return;
    }
  QString rawData = sDatas[0] + "+" + sDatas[1] + "+" + sDatas[2];
  if(!emcc.VerifyMsg(sDatas[1],rawData,sDatas[3])){
      qDebug()<<"Cheat Data!!!";
      return;
    }
  NResult result;
  result.timeStamp = sDatas[0].toInt();
  result.pubKey = sDatas[1];
  result.id = emcc.calcAddress(sDatas[1]);
  result.data = sDatas[2];
  result.dataHash = QCryptographicHash::hash(result.data.toLatin1(),QCryptographicHash::Keccak_256);

  resultChain.Add(result.timeStamp, result);

  if(resultChain.consensusSize(result.timeStamp)>=netCapacity){
      SendResultToGame(result.timeStamp, resultChain.consensusData(result.timeStamp));
    }
}

void NCausationConsensus::BroadcastCauseToCCN(QString data)
{
  //TODO: UDP2CCN
}

void NCausationConsensus::SendResultToGame(quint64 timeStamp,QString data)
{
  //TODO: UDP2Game
}

QString NCausationConsensus::SignData(QString data, quint64 timeStamp)
{
  QString rawData = QString::number(timeStamp) + "+" +
      emcc.publicKeyString + "+" +
      data;
  auto sign = emcc.Sign(rawData);
  return rawData + "+" + sign;
}

