#include "ntimesynctester.h"

NTimeSyncTester::NTimeSyncTester(QObject *parent) : QObject(parent)
{
    pingStates.insert("User1",10);
    pingStates.insert("User2",10);
    pingStates.insert("User3",10);
    pingStates.insert("User4",10);
    pingStates.insert("User5",10);

    QObject::connect(&ts, &NTimeSync::Tick, this, &NTimeSyncTester::OnTick);
    QObject::connect(&testTimer, &QTimer::timeout, this, &NTimeSyncTester::OnRcvSimulation);
    QObject::connect(this, &NTimeSyncTester::TickRcvSimulation, &ts, &NTimeSync::RcvTick);
}

void NTimeSyncTester::SelfTest()
{
    ts.SetPingState(pingStates);
    ts.StartP2PSync(6000);
    testTimer.start(6050);
}

void NTimeSyncTester::OnTick(int frameNo)
{
    qDebug()<<__FUNCTION__<<frameNo;
}

void NTimeSyncTester::OnRcvSimulation()
{
    emit TickRcvSimulation("User1", ts.GetCurrentFrameNo());
    emit TickRcvSimulation("User2", ts.GetCurrentFrameNo());
    emit TickRcvSimulation("User3", ts.GetCurrentFrameNo());
    emit TickRcvSimulation("User4", ts.GetCurrentFrameNo());
    emit TickRcvSimulation("User5", ts.GetCurrentFrameNo());
}
