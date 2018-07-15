#include "np2pringnet.h"

NP2PRingNet::NP2PRingNet(QObject *parent) : QObject(parent)
{

}

void NP2PRingNet::peerJoinCall(NodeInfo peerInfo)
{
    if(peerInfoHashMap.contains(peerInfo.addr)){
        peerInfoHashMap[peerInfo.addr].HeartBeat();
        return;
    }
    peerAddrList.append(peerInfo.addr);
    peerInfoHashMap.insert(peerInfo.addr,peerInfo);
}

void NP2PRingNet::update()
{
    broadcastNeighberPeersList();
    clearDeadPeers();
}

QByteArrayList NP2PRingNet::getAllAddress()
{
    return peerInfoHashMap.keys();
}

NodeInfo NP2PRingNet::getNodeInfo(QByteArray addr)
{
    return peerInfoHashMap[addr];
}

void NP2PRingNet::broadcastNeighberPeersList()
{
    auto itr = peerAddrList.begin();
    while(itr != peerAddrList.end()){
        auto ip = peerInfoHashMap[*itr].nat;
        auto neighboursData = getPeersNeighbourList(itr);
        qDebug()<<__FUNCTION__<<ip.ToString()<<neighboursData;
        emit Send(*itr,ip,neighboursData);
        if(!peerInfoHashMap[*itr].CheckAlive()){
            deadPeers.append(*itr);
        }
        itr++;
    }
}

QString NP2PRingNet::getPeersNeighbourList(QLinkedList<QByteArray>::iterator itr)
{
    QStringList neighbourList;
    QByteArrayList brl;
    brl.append(*itr);//ignore itself
    auto tItr = itr;
    for(int i = 0;i<neighbourRange;i++){
        tItr = next(tItr);
        if(!brl.contains(*tItr)){
            brl.append(*tItr);
            neighbourList.append(peerInfoHashMap[*tItr].ToString());
        }
    }
    tItr = itr;
    for(int i = 0;i<neighbourRange;i++){
        tItr = prev(tItr);
        if(!brl.contains(*tItr)){
            brl.append(*tItr);
            neighbourList.append(peerInfoHashMap[*tItr].ToString());
        }
    }
    return neighbourList.join(";");
}

void NP2PRingNet::clearDeadPeers()
{
    foreach(auto peerAddr, deadPeers){
        peerInfoHashMap.remove(peerAddr);

        auto itr = peerAddrList.begin();
        while(itr != peerAddrList.end()){
            if(peerAddr == *itr){
                peerAddrList.erase(itr);
                break;
            }
            itr++;
        }
    }
    deadPeers.clear();
}

QLinkedList<QByteArray>::iterator NP2PRingNet::next(QLinkedList<QByteArray>::iterator iitr)
{
    QLinkedList<QByteArray>::iterator itr = iitr;
    if(peerAddrList.size()>0){
        if(itr == peerAddrList.end()-1){
            itr = peerAddrList.begin();
            return itr;
        }
        itr++;
        return itr;
    }
    return itr;
}

QLinkedList<QByteArray>::iterator NP2PRingNet::prev(QLinkedList<QByteArray>::iterator iitr)
{
    QLinkedList<QByteArray>::iterator itr = iitr;
    if(peerAddrList.size()>0){
        if(itr == peerAddrList.begin()){
            itr = peerAddrList.end()-1;
            return itr;
        }
        itr--;
        return itr;
    }
    return itr;
}

void NP2PRingNet::SelfTest()
{
    NP2PRingNet net;
    net.peerJoinCall(NodeInfo("010101",QIPEndPoint("127.0.0.1:10000"),QIPEndPoint("127.10.0.1:10000")));
    net.peerJoinCall(NodeInfo("010102",QIPEndPoint("127.0.0.2:10000"),QIPEndPoint("127.10.0.2:10000")));
    net.peerJoinCall(NodeInfo("010103",QIPEndPoint("127.0.0.3:10000"),QIPEndPoint("127.10.0.3:10000")));
    net.peerJoinCall(NodeInfo("010104",QIPEndPoint("127.0.0.4:10000"),QIPEndPoint("127.10.0.4:10000")));
    net.peerJoinCall(NodeInfo("010105",QIPEndPoint("127.0.0.5:10000"),QIPEndPoint("127.10.0.5:10000")));
    net.peerJoinCall(NodeInfo("010106",QIPEndPoint("127.0.0.6:10000"),QIPEndPoint("127.10.0.6:10000")));
    net.peerJoinCall(NodeInfo("010107",QIPEndPoint("127.0.0.7:10000"),QIPEndPoint("127.10.0.7:10000")));
    net.peerJoinCall(NodeInfo("010108",QIPEndPoint("127.0.0.8:10000"),QIPEndPoint("127.10.0.8:10000")));
    net.update();
}