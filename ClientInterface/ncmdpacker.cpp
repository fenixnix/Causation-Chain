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
    packer.Push("0","a");
    packer.Push("1","b");
    packer.Push("2","c");
    packer.Push("3","d");
    packer.Push("4","e");
    packer.Push("5","f");
    packer.Push("6","g");
    packer.Push("7","h");
    qDebug()<<packer.PackJsonString();
    packer.Clear();
    packer.Push("7","h");
    packer.Push("6","g");
    packer.Push("5","f");
    packer.Push("4","e");
    packer.Push("3","d");
    packer.Push("2","c");
    packer.Push("1","b");
    packer.Push("0","a");
    qDebug()<<packer.PackJsonString();
    packer.Clear();
    packer.Push("0","a");
    packer.Push("2","c");
    packer.Push("4","e");
    packer.Push("6","g");
    packer.Push("1","b");
    packer.Push("3","d");
    packer.Push("5","f");
    packer.Push("7","h");
    qDebug()<<packer.PackJsonString();
    packer.Clear();
    packer.Push("0","a");
    packer.Push("2","c");
    packer.Push("4","e");
    packer.Push("6","g");
    packer.Push("1","b");
    packer.Push("3","d");
    packer.Push("5","f");
    packer.Push("7","h");
    qDebug()<<packer.PackJsonString();
}
