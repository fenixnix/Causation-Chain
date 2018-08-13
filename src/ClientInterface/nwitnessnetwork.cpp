#include "nwitnessnetwork.h"
#include "utility.h"

NWitnessNetwork::NWitnessNetwork(QObject *parent) : QObject(parent)
{
    connect(&p2p, &NP2PNode::RcvMsg, this, &NWitnessNetwork::OnRcvMsg);
}

void NWitnessNetwork::Init(QString jsonString)//members: id,pubKey,locEP,natEP
{
    auto array = QJsonDocument::fromJson(jsonString.toLatin1()).array();
    Init(array);
}

void NWitnessNetwork::Init(QJsonArray json)
{
    foreach(auto obj, json){
        p2p.AddPeerJson(obj.toObject());
        auto id = obj["id"].toString();
        auto pubKey = obj["pubKey"].toString();
        articipatorsKeyMap.insert(id, pubKey);
    }
}

QString NWitnessNetwork::GetConsensusCause(int frame)
{
    return consensus.CauseConsensus(frame);
}

void NWitnessNetwork::SetLocalCause(int frame, QString cause)
{
    QJsonObject obj;
    obj["cmd"] = "input";
    obj["frm"] = frame;
    obj["id"] = crypto.getAddr();
    obj["dat"] = cause;
    QJsonObject objs(obj);
    objs.remove("cmd");
    packer.Push(objs);
    auto jsonString = JSON2STRING(obj);
    p2p.broadcastMsg(jsonString);
}

void NWitnessNetwork::Sync(int frame)
{
    QJsonObject obj;
    obj["cmd"] = "inputPack";
    obj["id"] = crypto.getAddr();
    obj["dat"] = packer.PackJson();
    auto jsonString = JSON2STRING(obj);
    p2p.broadcastMsg(jsonString);
    //TODO:check data
    consensus.RcvCause(frame, crypto.getAddr(), jsonString);
}

void NWitnessNetwork::OnRcvMsg(QString msg)
{
    auto obj = STRING2JSON(msg);
    auto cmd = obj["cmd"];
    if(cmd == "input"){
        QJsonObject objs(obj);
        objs.remove("cmd");
        packer.Push(objs);
    }
    if(cmd == "inputPack"){
        //TODO:check data
        //consensus.RcvCause(frm,id,data);
        //TODO:make consensus;
    }
}
