#include "nemcc.h"
#include "emcclib.h"
#include <QCryptographicHash>
#include <QDebug>

NEmcc::NEmcc()
{

}

QString Hash256(QString data){
  auto hashData = QCryptographicHash::hash(data.toLocal8Bit(),QCryptographicHash::Sha256);
  return hashData.toHex();
}

void NEmcc::GenerateKeyPair()
{
  char tmpData[192];
  int length;
  GetKeyPair(tmpData,length);
  QByteArray br(tmpData,length);
  privateKeyString = QString(br.left(64));
  publicKeyString = QString(br.right(128));
  priKey.clear();
  pubKey.clear();
  priKey = QByteArray::fromHex(privateKeyString.toLatin1());
  pubKey = QByteArray::fromHex(publicKeyString.toLatin1());
  address = calcAddress(pubKey);
  qDebug()<<"Address:"<<address<<address.size();
}

QString NEmcc::Sign(QString data)
{
  auto hashData = Hash256(data);
  char tmpData[128];
  int length;
  GetSignData(privateKeyString.toLocal8Bit().data(),hashData.toLocal8Bit().data(),tmpData,length);
  QByteArray br(tmpData,length);
  return QString(br);
}

bool NEmcc::VerifyMsg(QString pubKey, QString msg, QString sign)
{
  auto hashData = Hash256(msg);
  qDebug()<<"Hash Data:"<<QString(hashData);
  return Verify(pubKey.toLocal8Bit().data(),
                hashData.toLocal8Bit().data(),
                sign.toLocal8Bit().data());
}

QString NEmcc::calcAddress(QByteArray pub)
{
  qDebug()<<pub.size()<<pub;
  auto pubhash = QCryptographicHash::hash(pub,QCryptographicHash::Keccak_256);
  qDebug()<<pubhash.size()<<pubhash;
  auto ripeMD = RIPEMD_160(pubhash);
  qDebug()<<ripeMD.size();
  QByteArray WithNetID;
  WithNetID.append((char)0);
  WithNetID.append(ripeMD);
  auto haha = QCryptographicHash::hash(QCryptographicHash::hash(WithNetID,QCryptographicHash::Keccak_256),QCryptographicHash::Keccak_256);
  auto first4 = haha.left(4);
  QByteArray finalData;
  finalData.append(WithNetID);
  finalData.append(first4);
  return QString(finalData.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
}

QString NEmcc::calcAddress(QString pub)
{
  return calcAddress(QByteArray::fromHex(pub.toLatin1()));
}

QByteArray NEmcc::RIPEMD_160(QByteArray in)
{
  return QCryptographicHash::hash(in,QCryptographicHash::Sha1);
}
