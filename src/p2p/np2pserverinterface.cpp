#include "np2pserverinterface.h"
#include "messageprotocol.h"

NP2PServerInterface::NP2PServerInterface(QObject *parent) : QObject(parent)
{
    QObject::connect(&udp, &UdpNetwork::Rcv,
                     this, &NP2PServerInterface::OnUdp);
}

void NP2PServerInterface::Init(QIPEndPoint local, QIPEndPoint server)
{
    Init(local.IP(), server.IP(), server.Port());
}

void NP2PServerInterface::Init(QHostAddress local, QHostAddress server, quint16 port)
{
    udp.Listen(local,port);
    udp.SetSendEndPoint(server,port);
}

void NP2PServerInterface::Query(QString msg)
{
    udp.Send(msg);
}

void NP2PServerInterface::OnUdp(QString msg)
{
    MessageProtocol mp;
    auto cmd = mp.Decode(msg);
    if(cmd==""){
        return;
    }
    emit ServerMsg(cmd,mp.getData());
}
