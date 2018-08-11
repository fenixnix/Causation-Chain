#ifndef NCMDPACKER_H
#define NCMDPACKER_H

#include <QHash>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class NCausePacker
{
public:
    NCausePacker();
    void Push(QJsonObject obj);
    void Push(int frame, QString addrID, QString cause);//every addr only can push once at first time
    void Clear();
    int Size();
    int frame = 0;
    QString PackJsonString();
    QJsonArray PackJson();

    static void SelfTest();//test pack hash order

private:
    QHash<QString, QString> cmdPack;
    QMap<QString, QJsonObject> jsonPack;

    QStringList list;
    QJsonArray jsonArr;
};

#endif // NCMDPACKER_H
