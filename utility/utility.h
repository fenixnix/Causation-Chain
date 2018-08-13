#ifndef UTILITY_H
#define UTILITY_H

#define CHECK_RETURN(X) if(X){qDebug()<<__FUNCTION__<<__LINE__;return;}
#define JSON2STRING(X) QString(QJsonDocument(X).toJson(QJsonDocument::Compact))
#define STRING2JSON(X) QJsonDocument::fromJson(X.toLatin1()).object()

#include <chrono>
using namespace std::chrono;

class StopWatch
{
public:
    StopWatch();
    void Start();
    float Count();
private:
    time_point<steady_clock> tp;
};

#endif // UTILITY_H

