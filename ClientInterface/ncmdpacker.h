#ifndef NCMDPACKER_H
#define NCMDPACKER_H

#include <QHash>
#include <QDebug>

class NCmdPacker
{
public:
    NCmdPacker();
    void Push(QString addr, QString cmd);//every addr only can push once at first time
    void Clear();
    int Size();
    int frame = 0;
    QString PackJsonString();

    static void SelfTest();//test pack hash order

private:
    QHash<QString, QString> cmdPack;

};

#endif // NCMDPACKER_H
