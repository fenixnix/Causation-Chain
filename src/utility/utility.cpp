#include "utility.h"
#include <numeric>
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
    auto now = steady_clock::now();
    auto difT = now - lastUpdateTime;
    lastUpdateTime = now;
    auto ms = toMs(difT);
    intervals.push_back(ms);
    recentlyIntervals.push_back(ms);
    if(recentlyIntervals.size()>recentCount){
        recentlyIntervals.pop_front();
    }
    intervalSum += ms;
    cout<<ms<<"ms,"<<"mean:"<<intervalSum/(float)intervals.size()<<" recentMean:"<<RecentlyMean()<<endl;
    return ms;
}

float StopWatch::Count()
{   
    auto now = steady_clock::now();
    auto difT = now - startTime;
    lastUpdateTime = now;
    return (float)difT.count()/1000000.0f;
}

void StopWatch::Print()
{
    cout<<"Stop Watch Interval Datas:"<<endl;
    float sum = 0;
    auto itr = intervals.begin();
    int cnt = 0;
    while(itr != intervals.end()){
        float iv = *itr;
        sum += iv;
        cout<<iv<<"ms,"<<"mean:"<<iv/(float)(cnt+1)<<" recentMean:"<<RecentlyMean()<<endl;
        cnt++;
        itr++;
    }
}

float StopWatch::RecentlyMean()
{
    float sum = std::accumulate(recentlyIntervals.begin(), recentlyIntervals.end(), 0.0f);
    if(recentlyIntervals.size()>recentCount){
        return sum/(float)recentlyIntervals.size();
    }
    return sum/recentCount;
}

float StopWatch::toMs(steady_clock::duration input)
{
    return (float)input.count()/1000000.0f;
}
