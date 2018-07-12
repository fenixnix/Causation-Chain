#include "netsync.h"
#include <QJsonDocument>

NetSync::NetSync(QObject *parent) : QObject(parent)
{
    QObject::connect(&p2p,&NP2PNode::neighbourListUpdate,this,&NetSync::PeerListUpdate);
    Init();
}

void NetSync::Init(QString priKey, QString pubKey)
{
    ecDsa.SetPriKey(priKey);
    ecDsa.SetPubKey(pubKey);

    QSettings setting("config",QSettings::IniFormat);
    auto id = ecDsa.ethAddr;
    //qDebug()<<"ID:"<<id;
    p2p.setID(id);


    QIPEndPoint local(setting.value("Local").toString());
    QIPEndPoint nat(setting.value("NATServer").toString());
    QIPEndPoint p2pA(setting.value("P2PServer").toString());

    if(local.IP().toString() == "127.0.0.1"){
        p2p.bindLocalEndPoint(QIPEndPoint(QHostAddress(NP2PNode::getLocalIP2()),local.Port()));
    }else{
        p2p.bindLocalEndPoint(QIPEndPoint(local.IP(),local.Port()));
    }

    p2p.setP2PServer(p2pA);
    p2p.join(nat);

    QObject::connect(&p2p,&NP2PNode::RcvMsg,this,&NetSync::RcvP2pMsg);
}

void NetSync::Init()
{
    ecDsa.GenerateKeyPair();
    Init(ecDsa.privateKeyString,ecDsa.publicKeyString);
}

QStringList NetSync::RequirePeerList()
{
    return CheckEthAddrList(p2p.neighbourList());
}

bool NetSync::PeerIsExist(QString peerAddress)
{
    return p2p.neighbourList().contains(peerAddress);
}

void NetSync::BroadcastBlockChainLevel(QString id, QString level)
{
    QJsonObject obj;
    obj.insert("ID",id);
    obj.insert("Addr",ecDsa.ethAddr);
    obj.insert("Level",level);
    QJsonDocument jdom(obj);
    QString msg = QString(jdom.toJson());
    QString signedMsg = setUpSignedMsg(msg);
    p2p.boardcastMsg(signedMsg);
    //return signedMsg;
}

void NetSync::RequireBlockChainData(QString id, QString nodeAddress, QString start, QString end)
{
    QJsonObject obj;
    obj.insert("ID",id);
    obj.insert("Addr",ecDsa.ethAddr);
    obj.insert("Start",start);
    obj.insert("End",end);
    QJsonDocument jdom(obj);
    QString msg = QString(jdom.toJson());
    QString signedMsg = setUpSignedMsg(msg);
    p2p.sendbyID(signedMsg,nodeAddress);
    //return signedMsg;
}

void NetSync::SendBlockChainData(QString id, QString nodeAddress, QString data)
{
    QJsonObject obj;
    obj.insert("ID",id);
    obj.insert("Addr",ecDsa.ethAddr);
    obj.insert("Data",data);
    QJsonDocument jdom(obj);
    QString msg = QString(jdom.toJson());
    QString signedMsg = setUpSignedMsg(msg);
    p2p.sendbyID(signedMsg,nodeAddress);
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

void NetSync::RcvP2pMsg(QString signedMsg)
{
    QJsonDocument jDom = QJsonDocument::fromJson(signedMsg.toLatin1());
    //qDebug()<<__FUNCTION__<<QString(jDom.toJson());
    auto msg = jDom["Msg"].toString();
    auto pubKey = jDom["PubKey"].toString();
    auto sign = jDom["Sign"].toString();
    if(!ecDsa.VerifyMsg(pubKey,msg,sign)){
        return;
    }

    QJsonObject obj = QJsonDocument::fromJson(msg.toLatin1()).object();
    QString addr = obj["Addr"].toString();
    QString contractID = obj["ID"].toString();
    if(obj.contains("Level")){
        emit RcvBlockChainLevel(contractID,addr,obj["Level"].toString());
        //qDebug()<<"Rcv:Level"<<contractID<<addr<<obj["Level"].toString();
    }
    if(obj.contains("Start")){
        emit RcvBlockChainDataRequire(contractID,addr,obj["Start"].toString(),obj["End"].toString());
        //qDebug()<<"Rcv:Require"<<contractID<<addr<<obj["Start"].toString()<<obj["End"].toString();
    }
    if(obj.contains("Data")){
        emit RcvBlockChainData(contractID,addr,obj["Data"].toString());
        //qDebug()<<"Rcv:Data"<<contractID<<addr<<obj["Data"].toString();
    }
}

void NetSync::PeerListUpdate(QStringList list)
{
    foreach(auto l, list){
        prevAllPeerList.removeAll(l);
    }
    //TODO:
    //emit UpdatePeerList(CheckEthAddrList(list),prevAllPeerList);
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
