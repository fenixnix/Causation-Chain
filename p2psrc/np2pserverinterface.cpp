#include "np2pserverinterface.h"
#include "messageprotocol.h"

NP2PServerInterface::NP2PServerInterface(QObject *parent) : QObject(parent)
{

}

void NP2PServerInterface::Init(QHostAddress ip, quint16 port)
{
    QObject::connect(&udp, &UdpNetwork::Rcv,
                     this, &NP2PServerInterface::OnUdp);
    udp.Listen(ip,port);
    udp.SetSendEndPoint(ip,port);
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
    emit P2PMsg(cmd,mp.getData());
}
