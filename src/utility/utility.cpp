#include "utility.h"
#include <iostream>

using namespace std;

StopWatch::StopWatch()
{
    Reset();
}

void StopWatch::Reset()
{
    startTime = steady_clock::now();
    intervals.clear();
    intervalSum = 0;
}

float StopWatch::Tick()
{
    auto difT = steady_clock::now() - lastUpdateTime;
    lastUpdateTime = now;
    auto ms = toMs(difT);
    intervals.push_back(ms);
    recentlyIntervals.push(ms);
    if(recentlyIntervals.size()>recentCount){
        recentlyIntervals.pop();
    }
    intervalSum += ms;
    cout<<ms<<"ms,"<<"mean:"<<ms/(float)intervals.size()<<endl;
    return ms;
}

float StopWatch::Count()
{   
    auto difT = steady_clock::now() - startTime;
    lastUpdateTime = now;
    return (float)difT.count/1000000.0f;
}

void StopWatch::Print()
{
    cout<<"Stop Watch Interval Datas:"<<endl;
    float sum = 0;
    for(int i = 0;i<intervals.size();i++){
        auto iv = intervals[i];
        sum += iv;
        cout<<iv<<"ms,"<<"mean:"<<iv/(float)(i+1)<<endl;
    }
}

float StopWatch::RecentlyMean()
{
    float sum = 0;
    for(int i = 0;i<recentlyIntervals.size();i++){
        auto iv = recentlyIntervals[i];
        sum += iv;
    }
    return sum/(float)recentlyIntervals.size();
}

float StopWatch::toMs(input)
{
    return (float)input.count/1000000.0f;
}
