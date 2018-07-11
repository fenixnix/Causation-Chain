#include "ncausationconsensus.h"
#include <QDateTime>
#include <QCryptographicHash>

NCausationConsensus::NCausationConsensus(QObject *parent) : QObject(parent)
{

}

void NCausationConsensus::RcvCause(quint64 timeStamp, QString id, QString data)
{
    causeChain.Add(NCause(timeStamp, id, data));
}

void NCausationConsensus::RcvResult(quint64 timeStamp, QString id, QString data)
{
    resultChain.Add(NResult(timeStamp, id, data));
    if(resultChain.consensusSize(timeStamp)>=netCapacity){
        emit ReachConsensus(timeStamp, resultChain.consensusData(timeStamp));
    }
}

//void NCausationConsensus::RcvCauseFromLocal(QString id, QString data)
//{
//  quint64 timeStamp = QDateTime::currentMSecsSinceEpoch();
//  RcvCause(timeStamp, id, data);
////  SendCauseToLocal(timeStamp, data);
////  QString msg = SignData(data,timeStamp);
////  BroadcastCauseToCCN(msg);
//}

//void NCausationConsensus::RcvCauseFromCCN(QString sData)
//{
//  auto sDatas = sData.split("+");
//  if(sDatas.size()<4){
//      qDebug()<<"Error Cause!!!";
//      return;
//    }
//  QString rawData = sDatas[0] + "+" + sDatas[1] + "+" + sDatas[2];
//  if(!emcc.VerifyMsg(sDatas[1],rawData,sDatas[3])){
//      qDebug()<<"Cheat Data!!!";
//      return;
//    }
//  NCause cause;
//  cause.timeStamp = sDatas[0].toInt();
//  cause.pubKey = sDatas[1];
//  cause.id = emcc.calcAddress(sDatas[1]);
//  cause.setData(sDatas[2]);
//  causeChain.Add(cause.timeStamp, cause);

//  SendCauseToLocal(cause.timeStamp, cause.getData());
//}

//void NCausationConsensus::RcvResultFromLocal(QString data)
//{
//  QStringList pair = data.split("+");
//  if(pair.size()!=2){
//      qDebug()<<"error rcv msg!!!";
//      return;
//    }
//  QString msg = SignData(pair[0],pair[1].toInt());
//  BroadcastCauseToCCN(msg);
//}

//void NCausationConsensus::SendCauseToLocal(quint64 timeStamp, QString data)
//{
//  //TODO: Send timeStamp + cause data to UserClient
//  interface.SendCause(QString::number(timeStamp) + "+" + data);
//}

//void NCausationConsensus::SendResultToLocal(quint64 timeStamp,QString data)
//{
//  //TODO: Send timeStamp + result data to UserClient
//  interface.SendResult(QString::number(timeStamp) + "+" + data);
//}

//void NCausationConsensus::RcvResultFromCCN(QString sData)
//{
//  auto sDatas = sData.split("+");
//  if(sDatas.size()<4){
//      qDebug()<<"Error Cause!!!";
//      return;
//    }
//  QString rawData = sDatas[0] + "+" + sDatas[1] + "+" + sDatas[2];
//  if(!emcc.VerifyMsg(sDatas[1],rawData,sDatas[3])){
//      qDebug()<<"Cheat Data!!!";
//      return;
//    }
//  NResult result;
//  result.timeStamp = sDatas[0].toInt();
//  result.pubKey = sDatas[1];
//  result.id = emcc.calcAddress(sDatas[1]);
//  result.setData(sDatas[2]);

//  resultChain.Add(result);

//  if(resultChain.consensusSize(result.timeStamp)>=netCapacity){
//      SendResultToLocal(result.timeStamp, resultChain.consensusData(result.timeStamp));
//  }
//}

QString NCausationConsensus::SelfTest()
{
    QString txt;
    QTextStream ts(&txt);
    ts<<NCauseChain::SelfTest();
    ts<<NResultChain::SelfTest();
    return txt;
}

//void NCausationConsensus::BroadcastCauseToCCN(QString data)
//{
//  //TODO: UDP2CCN
//}

//QString NCausationConsensus::SignData(QString data, quint64 timeStamp)
//{
//  QString rawData = QString::number(timeStamp) + "+" +
//      emcc.publicKeyString + "+" +
//      data;
//  auto sign = emcc.Sign(rawData);
//  return rawData + "+" + sign;
//}
