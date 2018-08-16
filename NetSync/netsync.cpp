#include "netsync.h"
#include <QJsonDocument>

NetSync::NetSync(QObject *parent) : QObject(parent)
{
    connect(&p2p,&MainNetServer::P2PListUpdate,this,&NetSync::PeerListUpdate);
    connect(&p2p,&MainNetServer::RcvMsg,this,&NetSync::RcvP2pMsg);
    Init();
}

void NetSync::Init(QString secKey, QString pubKey)
{
    p2p.Init(secKey, pubKey);
    ecDsa.SetSecKey(secKey);
    ecDsa.SetPubKey(pubKey);
}

void NetSync::Init()
{
    ecDsa.GenerateKeyPair();
    Init(ecDsa.privateKeyString,ecDsa.publicKeyString);
}

QStringList NetSync::neighbourPeerList()
{
    return CheckEthAddrList(p2p.getNeighbourList());
}

bool NetSync::PeerIsNeighbour(QString peerAddress)
{
    return p2p.getNeighbourList().contains(peerAddress);
}

void NetSync::onBroadcastBlockChainLevel(QString id, QString level)
{
    QJsonObject obj;
    obj.insert("ID",id);
    obj.insert("Addr",ecDsa.ethAddr);
    obj.insert("Level",level);
    QJsonDocument jdom(obj);
    QString msg = QString(jdom.toJson());
    QString signedMsg = setUpSignedMsg(msg);
    p2p.BroadcastMsg(signedMsg);
    //return signedMsg;
}

void NetSync::onRequireBlockChainData(QString id, QString nodeAddress, QString start, QString end)
{
    QJsonObject obj;
    obj.insert("ID",id);
    obj.insert("Addr",ecDsa.ethAddr);
    obj.insert("Start",start);
    obj.insert("End",end);
    QJsonDocument jdom(obj);
    QString msg = QString(jdom.toJson());
    QString signedMsg = setUpSignedMsg(msg);
    p2p.SendMsg(signedMsg,nodeAddress);
    //return signedMsg;
}

void NetSync::onSendBlockChainData(QString id, QString nodeAddress, QString data)
{
    QJsonObject obj;
    obj.insert("ID",id);
    obj.insert("Addr",ecDsa.ethAddr);
    obj.insert("Data",data);
    QJsonDocument jdom(obj);
    QString msg = QString(jdom.toJson());
    QString signedMsg = setUpSignedMsg(msg);
    p2p.SendMsg(signedMsg,nodeAddress);
    //return signedMsg;
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
    QJsonObject obj;
    obj.insert("ID",contractID);
    obj.insert("Addr",ecDsa.ethAddr);
    obj.insert("CMD",cmd);
    obj.insert("Dat",data);
    QJsonDocument jdom(obj);
    QString msg = QString(jdom.toJson());
    QString signedMsg = setUpSignedMsg(msg);
    p2p.SendMsg(signedMsg,addr);
}

void NetSync::onOnnBroadcast(QString contractID, QString cmd, QString data)
{
    QJsonObject obj;
    obj.insert("ID",contractID);
    obj.insert("Addr",ecDsa.ethAddr);
    obj.insert("CMD",cmd);
    obj.insert("Dat",data);
    QJsonDocument jdom(obj);
    QString msg = QString(jdom.toJson());
    QString signedMsg = setUpSignedMsg(msg);
    p2p.BroadcastMsg(signedMsg);
}

void NetSync::onSendRequire(QString id, QByteArray addr, QString data)
{
    QJsonObject obj;
    obj.insert("ID",id);
    obj.insert("Addr",ecDsa.ethAddr);
    obj.insert("Require",data);
    QJsonDocument jdom(obj);
    QString msg = QString(jdom.toJson());
    QString signedMsg = setUpSignedMsg(msg);
    p2p.SendMsg(signedMsg,addr);
}

void NetSync::RcvP2pMsg(QString signedMsg)
{
    QJsonDocument jDom = QJsonDocument::fromJson(signedMsg.toLatin1());
    //qDebug()<<__FUNCTION__<<QString(jDom.toJson());
    auto msg = jDom["Msg"].toString();
    auto pubKey = jDom["PubKey"].toString();
    auto sign = jDom["Sign"].toString();
    if(!ecDsa.VerifyMsg(pubKey,msg,sign)){
        qDebug()<<pubKey<<msg<<sign;
        return;
    }

    QJsonObject obj = QJsonDocument::fromJson(msg.toLatin1()).object();
    QString addr = obj["Addr"].toString();
    QString contractID = obj["ID"].toString();
    if(obj.contains("Level")){
        emit doRcvBlockChainLevel(contractID,addr,obj["Level"].toString());
        //qDebug()<<"Rcv:Level"<<contractID<<addr<<obj["Level"].toString();
    }

    if(obj.contains("Start")){
        emit doRcvBlockChainDataRequire(contractID,addr,obj["Start"].toString(),obj["End"].toString());
        //qDebug()<<"Rcv:Require"<<contractID<<addr<<obj["Start"].toString()<<obj["End"].toString();
    }

    if(obj.contains("Data")){
        emit doRcvBlockChainData(contractID,addr,obj["Data"].toString());
        //qDebug()<<"Rcv:Data"<<contractID<<addr<<obj["Data"].toString();
    }

    if(obj.contains("Require")){
        emit doRcvRequire(contractID,addr,obj["Require"].toString());
        //qDebug()<<"Rcv:Data"<<contractID<<addr<<obj["Data"].toString();
    }

    if(obj.contains("CMD")){
        emit doOnnRequire(contractID,addr,obj["CMD"].toString(),obj["Dat"].toString());
    }
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

    emit doUpdatePeerList(CheckEthAddrList(list),prevAllPeerList,newComerList);
    prevAllPeerList = list;
}

QString NetSync::setUpSignedMsg(QString msg)
{
    //qDebug()<<__FUNCTION__<<msg;
    QString msgHashSign = ecDsa.Sign(msg);
    QJsonObject obj;
    obj.insert("Msg",msg);
    obj.insert("Sign",msgHashSign);
    obj.insert("PubKey",ecDsa.publicKeyString);
    QJsonDocument jdom(obj);
    QString jsonString = QString(jdom.toJson());
    //qDebug()<<jsonString;
    return jsonString;
}

QStringList NetSync::CheckEthAddrList(QStringList list)
{
    QStringList result;
    foreach(auto l, list){
        if(NEmcc::CheckEthAddr(l)){
            result.append(l);
        }
    }
    return result;
}
