#include "ntimesync.h"

NTimeSync::NTimeSync(QObject *parent) : QObject(parent)
{
    QObject::connect(&timer,&QTimer::timeout,this,&NTimeSync::OnTimeElapse);
    deadLineTimer.setSingleShot(true);//Only Trig Once
    QObject::connect(&deadLineTimer, &QTimer::timeout, this &NTimeSync::OnDeadLineElapse);
}

void NTimeSync::Start(int interval)
{
    this->interval = interval;
    timer.start(interval);
}

int NTimeSync::GetCurrentFrameNo()
{
    return frameNo;
}

void NTimeSync::RcvTick(QString addr, int frameNo)
{
    if(frameNo!=this->frameNo){
        return;
    }
    if(timePoints.contains(addr)){
        return;
    }else{
        timePoints.insert(addr,steady_clock::now());
    }
    timePoints[addr] -= pingState[addr]/2;

    if(timePoints.size()>=pingState.size()%0.8){
        CalcBias();
    }
}

void NTimeSync::OnTimeElapse()
{
    myTimePoint = steady_clock::now();
    emit Tick(frameNo);
    deadLineTimer.start(2000);
}

void NTimeSync::OnDeadLineElapse()
{
    CalcBias();
}

void NTimeSync::CalcBias()
{
    qDebug()<<__FUNCTION__;
    long long mean;
    foreach(auto v, timePoints){
        mean += v/timePoints.size();
    }
    qDebug()<<mean;
    auto bias = myTimePoint - mean;
    timer.setInterval(interval+bias/1000000);
    frameNo++;
    timePoints.clear();
}
