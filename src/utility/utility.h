#ifndef UTILITY_H
#define UTILITY_H

#define CHECK_RETURN(X) if(X){qDebug()<<__FUNCTION__<<__LINE__;return;}
#define JSON2STRING(X) QString(QJsonDocument(X).toJson(QJsonDocument::Compact))
#define STRING2JSON(X) QJsonDocument::fromJson(X.toLatin1()).object()

#include <chrono>
#include <list>
#include <queue>
using namespace std::chrono;

class StopWatch
{
public:
    StopWatch();
    void Reset();
    float Tick();
    float Count();
    void Print();
    float RecentlyMean();
private:
    float toMs(auto input);
    time_point<steady_clock> startTime;
    time_point<steady_clock> lastUpdateTime;
    std::list<float> intervals;
    std::queue<float> recentlyIntervals;
    int recentCount = 10;
    float intervalSum = 0;
};

#endif // UTILITY_H

