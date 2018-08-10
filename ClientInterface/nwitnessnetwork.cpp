#include "nwitnessnetwork.h"
#include "utility.h"

NWitnessNetwork::NWitnessNetwork(QObject *parent) : QObject(parent)
{
    connect(&p2p, &NP2PNode::RcvMsg, this, &NWitnessNetwork::OnRcvMsg);
}

void NWitnessNetwork::Init(QString jsonString)//members: id,pubKey,locEP,natEP
{
    auto array = QJsonDocument::fromJson(jsonString).array();
    Init(json);
}

void NWitnessNetwork::Init(QJsonArray json)
{
    foreach(auto obj, json){
        p2p.AddPeerJson(obj);
        auto id = obj["id"];
        auto pubKey = obj["pubKey"];
        articipatorsKeyMap.insert(id, pubKey);
    }
}

QString NWitnessNetwork::GetConsensusCause(int frame)
{

}

void NWitnessNetwork::SetLocalCause(int frame, QString cause)
{
    QJsonObject obj;
    obj["cmd"] = "input";
    obj["frm"] = frame;
    obj["id"] = crypto.getAddr();
    obj["dat"] = cause;
    packer.Push(QJsonObject(obj).remove("cmd"));
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
}

void NWitnessNetwork::OnRcvMsg(QString msg)
{
    auto obj = STRING2JSON(msg);
    auto cmd = obj["cmd"];
    if(cmd == "input"){
        packer.Push(QJsonObject(obj).remove("cmd"));
    }
    if(cmd == "inputPack"){
        //TODO:make consensus;
        //consensus.Push
    }
}
