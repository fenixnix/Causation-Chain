#ifndef NTIMESYNC_H
#define NTIMESYNC_H

#include <QObject>
#include <QTimer>
#include <QHash>
#include <chrono>
#include <QDebug>

using namespace std::chrono;

class NTimeSync : public QObject
{
    Q_OBJECT
public:
    explicit NTimeSync(QObject *parent = nullptr);
    void SetPingState(QHash<QString, int> states);
    void StartP2PSync(int interval);
    void StartServerSync();
    void StartTestSync(int interval);

    int GetCurrentFrameNo();

signals:
    void Tick(int frameNo);
    void TimeBiasMS(int bias);

public slots:
    void RcvTick(QString addr, int frameNo);
    void RcvServerTick();

private slots:
    void OnTestTime();
    void OnTimeElapse();
    void OnDeadLineElapse();

private:
    void CalcBias();

    steady_clock::time_point myTimePoint;
    QHash<QString, steady_clock::time_point> timePoints;
    QHash<QString, duration<long long, std::nano>> pingStates;//nano sec
    QTimer timer;
    QTimer deadLineTimer;
    int interval;
    int modifyMS;
    int frameNo = 0;
};

#endif // NTIMESYNC_H
