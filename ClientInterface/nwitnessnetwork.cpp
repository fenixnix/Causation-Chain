#include "nwitnessnetwork.h"

NWitnessNetwork::NWitnessNetwork(QObject *parent) : QObject(parent)
{

}

void NWitnessNetwork::Init(QString json)//members: id,pubKey,locEP,natEP
{
    auto array = QJsonDocument::fromJson(json).array();
    foreach(auto obj, array){

    }
}
