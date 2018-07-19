#include "ntimesynctester.h"

NTimeSyncTester::NTimeSyncTester(QObject *parent) : QObject(parent)
{
    pingStates.insert("User1",100000000);
    pingStates.insert("User2",200000000);
    pingStates.insert("User3",300000000);
    pingStates.insert("User4",400000000);
    pingStates.insert("User5",500000000);
    pingStates.insert("User6",600000000);
    pingStates.insert("User7",700000000);

    QObject::connect(&ts, &NTimeSync::Tick, this, &NTimeSyncTester::OnTick);
    QObject::connect(&testTimer, &QTimer::timeout, this, &NTimeSyncTester::OnRcvSimulation);
    QObject::connect(this, &NTimeSyncTester::TickRcvSimulation, &ts, &NTimeSync::RcvTick);
}

void NTimeSyncTester::SelfTest()
{
    ts.SetPingState(pingStates);
    ts.Start(6000);
    testTimer.start(6000);
}

void NTimeSyncTester::OnTick()
{
    qDebug()<<__FUNCTION__;
}

void NTimeSyncTester::OnRcvSimulation()
{
    emit TickRcvSimulation("User1", ts.GetCurrentFrameNo());
    emit TickRcvSimulation("User2", ts.GetCurrentFrameNo());
    emit TickRcvSimulation("User3", ts.GetCurrentFrameNo());
    emit TickRcvSimulation("User4", ts.GetCurrentFrameNo());
    emit TickRcvSimulation("User5", ts.GetCurrentFrameNo());
    emit TickRcvSimulation("User6", ts.GetCurrentFrameNo());
    emit TickRcvSimulation("User7", ts.GetCurrentFrameNo());
}
