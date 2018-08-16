#include "netsync.h"
#include <QJsonDocument>

NetSync::NetSync(QObject *parent) : QObject(parent)
{
    connect(&p2p,&MainNetServer::P2PListUpdate,this,&NetSync::PeerListUpdate);
    connect(&p2p,&MainNetServer::RcvMsg,this,&NetSync::RcvP2pMsg);
}

void NetSync::Init(QString addrID)
{
    p2p.Init(addrID);
}

QStringList NetSync::neighbourPeerList()
{
    return p2p.getNeighbourList();
    //return CheckEthAddrList(p2p.getNeighbourList());
}

bool NetSync::PeerIsNeighbour(QString peerAddress)
{
    return p2p.getNeighbourList().contains(peerAddress);
}

void NetSync::SelfTest()
{
//    auto msg0 = BoardcastBlockChainLevel("ONN","100");
//    auto msg1 = RequireBlockChainData("ONN","Node Addresss1","50","60");
//    auto msg2 = SendBlockChainData("ONN","NodeAddress2","BlockChainData");
//    RcvP2pMsg(msg0);
//    RcvP2pMsg(msg1);
    //    RcvP2pMsg(msg2);
}

void NetSync::onEnterSubNet(QString contractID)
{

}

void NetSync::onQuitSubNet(QString contractID)
{

}

void NetSync::onGetBossAddr(QByteArrayList bossList)
{
    //p2p.RequireNatbyAddr(bossList);
}

void NetSync::onQueuePeerStatebyAddr(QByteArrayList peerList)
{
    //TODO:Ask for Peer State from P2P server
}

void NetSync::onOnnRequire(QString contractID, QByteArray addr, QString cmd, QString data)
{
    auto msg = PackCMD(contractID,cmd,data);
    p2p.SendMsg(msg,addr);
}

void NetSync::onOnnBroadcast(QString contractID, QString cmd, QString data)
{
    auto msg = PackCMD(contractID,cmd,data);
    p2p.BroadcastMsg(msg);
}

QString NetSync::PackCMD(QString contractID, QString cmd, QString data)
{
    QJsonObject obj;
    obj["ID"] = contractID;
    obj["Addr"] = p2p.getID();
    obj["CMD"] = cmd;
    obj["Dat"] = data;
    QJsonDocument jdom(obj);
    QString msg = QString(jdom.toJson());
    return msg;
}

void NetSync::RcvP2pMsg(QString msg)
{
    QJsonDocument obj = QJsonDocument::fromJson(msg.toLatin1());
    QString addr = obj["Addr"].toString();
    QString contractID = obj["ID"].toString();
    emit doOnnRequire(contractID,addr,obj["CMD"].toString(),obj["Dat"].toString());
}

void NetSync::PeerListUpdate(QStringList list)
{
    QStringList newComerList;
    foreach (auto l, list) {
        if(!prevAllPeerList.contains(l)){
            newComerList.append(l);
        }
    }

    foreach(auto l, list){
        prevAllPeerList.removeAll(l);
    }

    //emit doUpdatePeerList(CheckEthAddrList(list),prevAllPeerList,newComerList);
    emit doUpdatePeerList(list,prevAllPeerList,newComerList);
    prevAllPeerList = list;
}

//QString NetSync::setUpSignedMsg(QString msg)
//{
//    //qDebug()<<__FUNCTION__<<msg;
//    QString msgHashSign = ecDsa.Sign(msg);
//    QJsonObject obj;
//    obj.insert("Msg",msg);
//    obj.insert("Sign",msgHashSign);
//    obj.insert("PubKey",ecDsa.publicKeyString);
//    QJsonDocument jdom(obj);
//    QString jsonString = QString(jdom.toJson());
//    //qDebug()<<jsonString;
//    return jsonString;
//}

//QStringList NetSync::CheckEthAddrList(QStringList list)
//{
//    QStringList result;
//    foreach(auto l, list){
//        if(NEmcc::CheckEthAddr(l)){
//            result.append(l);
//        }
//    }
//    return result;
//}
