#include "np2ppublicnetwork.h"

NP2PPublicNetwork::NP2PPublicNetwork(QObject *parent) : QObject(parent)
{
    iPtr = peers.begin();
}

QLinkedList<QString>::iterator NP2PPublicNetwork::next()
{
    if(peers.size()>0){
        if(iPtr == peers.end()-1){
            iPtr = peers.begin();
            return iPtr;
        }
        iPtr++;
        return iPtr;
    }
    return iPtr;
}

QLinkedList<QString>::iterator NP2PPublicNetwork::prev()
{
    if(peers.size()>0){
        if(iPtr == peers.begin()){
            iPtr = peers.end()-1;
            return iPtr;
        }
        iPtr--;
        return iPtr;
    }
    return iPtr;
}

void NP2PPublicNetwork::SelfTest()
{
    qDebug()<<__FUNCTION__;
    NP2PPublicNetwork net;
    net.peers.append("a");
    net.peers.append("b");
    net.peers.append("c");
    net.peers.append("d");
    net.peers.append("e");
    net.iPtr = net.peers.end()-1;
    //net.prev();
    qDebug()<<*(net.iPtr);
    net.prev();
    qDebug()<<*(net.iPtr);
    net.prev();
    qDebug()<<*(net.iPtr);
    net.prev();
    qDebug()<<*(net.iPtr);
    net.prev();
    qDebug()<<*(net.iPtr);
    net.prev();
    qDebug()<<*(net.iPtr);
    net.prev();
    qDebug()<<*(net.iPtr);
    net.prev();
    qDebug()<<*(net.iPtr);
    net.prev();
    qDebug()<<*(net.iPtr);
    net.prev();
    qDebug()<<*(net.iPtr);
    //qDebug()<<net.iPtr;
    qDebug()<<__FUNCTION__;
}
