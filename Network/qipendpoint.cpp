#include "qipendpoint.h"

QIPEndPoint::QIPEndPoint()
{

}

QIPEndPoint::QIPEndPoint(QString endPoint)
{
  Init(endPoint);
}

QIPEndPoint::QIPEndPoint(QHostAddress ip, qint16 port)
{
  addr = ip;
  this->port = port;
}

QIPEndPoint::QIPEndPoint(QString ip, qint16 port)
{
  Init(ip,port);
}

void QIPEndPoint::Init(QString endPoint)
{
  auto pair = endPoint.split(':');
  if(pair.size()<2){
      return;
  }
  Init(pair[0],pair[1].toInt());
}

void QIPEndPoint::Init(QString ip, qint16 port)
{
  addr = QHostAddress(ip);
  this->port = port;
}

QHostAddress QIPEndPoint::IP()
{
  return addr;
}

qint16 QIPEndPoint::Port()
{
  return port;
}

QString QIPEndPoint::ToString()
{
  return addr.toString()+":"+QString::number(port);
}
