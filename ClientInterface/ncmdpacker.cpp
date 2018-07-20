#include "ncmdpacker.h"

NCmdPacker::NCmdPacker()
{

}

void NCmdPacker::Push(QString addr, QString cmd)
{
    if(cmdPack.contains(addr)){
        return;
    }
    cmdPack.insert(addr, cmd);
}

void NCmdPacker::Clear()
{
    cmdPack.clear();
}

int NCmdPacker::Size()
{
    return cmdPack.size();
}

#include <QJsonDocument>
#include <QJsonObject>
QString NCmdPacker::PackJsonString()
{
    QJsonObject obj;
    foreach(auto cmd, cmdPack.keys()){
        obj.insert(cmd,cmdPack[cmd]);
    }
    return QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
}

void NCmdPacker::SelfTest()
{
    qDebug()<<__FUNCTION__;
    NCmdPacker packer;
    packer.Push("0","acmd");
    packer.Push("1","bcmd");
    packer.Push("2","ccmd");
    packer.Push("3","dcmd");
    packer.Push("4","ecmd");
    packer.Push("5","fcmd");
    packer.Push("6","gcmd");
    packer.Push("7","hcmd");
    qDebug()<<packer.PackJsonString();
    packer.Clear();
    packer.Push("7","ecmd");
    packer.Push("6","fcmd");
    packer.Push("5","gcmd");
    packer.Push("4","hcmd");
    packer.Push("3","acmd");
    packer.Push("2","bcmd");
    packer.Push("1","ccmd");
    packer.Push("0","dcmd");
    qDebug()<<packer.PackJsonString();
    packer.Clear();
    packer.Push("0","ecmd");
    packer.Push("2","fcmd");
    packer.Push("4","acmd");
    packer.Push("6","bcmd");
    packer.Push("1","gcmd");
    packer.Push("3","hcmd");
    packer.Push("5","ccmd");
    packer.Push("7","dcmd");
    qDebug()<<packer.PackJsonString();
    packer.Clear();
    packer.Push("0","ecmd");
    packer.Push("2","fcmd");
    packer.Push("4","acmd");
    packer.Push("6","bcmd");
    packer.Push("1","gcmd");
    packer.Push("3","hcmd");
    packer.Push("5","ccmd");
    packer.Push("7","dcmd");
    qDebug()<<packer.PackJsonString();
}
