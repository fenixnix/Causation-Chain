#ifndef NCONSENSUSBLOCK_H
#define NCONSENSUSBLOCK_H

#include <QtCore>

class NConsensusUnit
{
public:
    NConsensusUnit();
    NConsensusUnit(QByteArray hash, QString data);
    void AddMember(QString id);
    int Size();
    QByteArray datahash;
    QString data;
    QList<QString> members;

    QString Print();
};

#endif // NCONSENSUSBLOCK_H
