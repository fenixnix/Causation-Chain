#include "ntimesync.h"

NTimeSync::NTimeSync(QObject *parent) : QObject(parent)
{

}

void NTimeSync::SetPingState(QHash<QString, int> states)
{
    pingStates.clear();
    foreach(auto s, states.keys()){
        duration<long long ,std::nano> dur(states[s]*500000);
        pingStates.insert(s,dur);
    }
}

void NTimeSync::StartP2PSync(int interval)
{
    QObject::connect(&timer,&QTimer::timeout,this,&NTimeSync::OnTimeElapse);
    deadLineTimer.setSingleShot(true);//Only Trig Once
    QObject::connect(&deadLineTimer, &QTimer::timeout, this, &NTimeSync::OnDeadLineElapse);
    this->interval = interval;
    timer.start(interval);
}

void NTimeSync::StartTestSync(int interval)
{
    QObject::connect(&timer,&QTimer::timeout,this,&NTimeSync::OnTestTime);
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
    timePoints[addr] -= pingStates[addr];

//    if(timePoints.size()>=pingStates.size()*0.8){
//        qDebug()<<__FUNCTION__;
//        CalcBias();
    //    }
}

void NTimeSync::RcvServerTick()
{
    emit Tick(frameNo);
    frameNo++;
}

void NTimeSync::OnTestTime()
{
    emit Tick(frameNo);
    frameNo++;
}

void NTimeSync::OnTimeElapse()
{
    myTimePoint = steady_clock::now();
    emit Tick(frameNo);
    timer.stop();
    timer.start(interval + modifyMS);
    qDebug()<<__FUNCTION__<<interval;
    deadLineTimer.start(2000);
}

void NTimeSync::OnDeadLineElapse()
{
    CalcBias();
}

void NTimeSync::CalcBias()
{
    long long mean = 0;
    foreach(auto v, timePoints){
        mean += v.time_since_epoch().count()/timePoints.size();
    }
    qDebug()<<"my:"<<myTimePoint.time_since_epoch().count()/1000000<<"ms";
    qDebug()<<"mean:"<<mean/1000000<<"ms";
    auto bias = mean - myTimePoint.time_since_epoch().count();
    modifyMS = bias/1000000;
    //qDebug()<<"dif:"<<modifyMS/2;
    frameNo++;
    timePoints.clear();
}
