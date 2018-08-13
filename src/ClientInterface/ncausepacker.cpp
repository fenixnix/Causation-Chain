#include "ncausepacker.h"

NCausePacker::NCausePacker()
{

}

void NCausePacker::Push(QJsonObject obj)
{
    auto id = obj["id"].toString();
    if(list.contains(id))return;
    jsonArr<<obj;
}

void NCausePacker::Push(int frame, QString addrID, QString cause)
{
    if(cmdPack.contains(addrID)){
        return;
    }
    cmdPack.insert(addrID, cause);
}

void NCausePacker::Clear()
{
    cmdPack.clear();
    list.clear();
    jsonArr = QJsonArray();
}

int NCausePacker::Size()
{
    return cmdPack.size();
}

QString NCausePacker::PackJsonString()
{
    QJsonObject obj;
    foreach(auto cmd, cmdPack.keys()){
        obj.insert(cmd,cmdPack[cmd]);
    }
    return QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
}

QJsonArray NCausePacker::PackJson()
{
    return jsonArr;
}

void NCausePacker::SelfTest()
{
//    qDebug()<<__FUNCTION__;
//    NCausePacker packer;
//    packer.Push("0","a");
//    packer.Push("1","b");
//    packer.Push("2","c");
//    packer.Push("3","d");
//    packer.Push("4","e");
//    packer.Push("5","f");
//    packer.Push("6","g");
//    packer.Push("7","h");
//    qDebug()<<packer.PackJsonString();
//    packer.Clear();
//    packer.Push("7","h");
//    packer.Push("6","g");
//    packer.Push("5","f");
//    packer.Push("4","e");
//    packer.Push("3","d");
//    packer.Push("2","c");
//    packer.Push("1","b");
//    packer.Push("0","a");
//    qDebug()<<packer.PackJsonString();
//    packer.Clear();
//    packer.Push("0","a");
//    packer.Push("2","c");
//    packer.Push("4","e");
//    packer.Push("6","g");
//    packer.Push("1","b");
//    packer.Push("3","d");
//    packer.Push("5","f");
//    packer.Push("7","h");
//    qDebug()<<packer.PackJsonString();
//    packer.Clear();
//    packer.Push("0","a");
//    packer.Push("2","c");
//    packer.Push("4","e");
//    packer.Push("6","g");
//    packer.Push("1","b");
//    packer.Push("3","d");
//    packer.Push("5","f");
//    packer.Push("7","h");
//    qDebug()<<packer.PackJsonString();
}
