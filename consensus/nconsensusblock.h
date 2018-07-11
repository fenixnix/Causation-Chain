#ifndef NCONSENSUSBLOCK_H
#define NCONSENSUSBLOCK_H

#include <QtCore>

class NConsensusBlock
{
public:
    NConsensusBlock();
    NConsensusBlock(QByteArray hash, QString data);
    void AddMember(QString id);
    int Size();
    QByteArray datahash;
    QString data;
    QList<QString> members;

    QString Print();
};

#endif // NCONSENSUSBLOCK_H
