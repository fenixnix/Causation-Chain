#include "utility.h"

StopWatch::StopWatch()
{
    Start();
}

void StopWatch::Start()
{
    tp = steady_clock::now();
}

float StopWatch::Count()
{
    auto lastUpdateTime = steady_clock::now();
    auto difT = lastUpdateTime - tp;
    auto ping = difT.count();
    return (float)ping/1000000.0f;
}
