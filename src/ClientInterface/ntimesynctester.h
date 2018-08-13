#ifndef NTIMESYNCTESTER_H
#define NTIMESYNCTESTER_H

#include <QObject>
#include "ntimesync.h"

class NTimeSyncTester : public QObject
{
    Q_OBJECT
public:
    explicit NTimeSyncTester(QObject *parent = nullptr);
    void SelfTest();

signals:
    void TickRcvSimulation(QString addr, int frameNo);

public slots:

private slots:
    void OnTick(int frameNo);
    void OnRcvSimulation();
private:
    QHash<QString, int> pingStates; //ms
    QTimer testTimer;
    NTimeSync ts;
};

#endif // NTIMESYNCTESTER_H
