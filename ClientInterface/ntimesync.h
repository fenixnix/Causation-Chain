#ifndef NTIMESYNC_H
#define NTIMESYNC_H

#include <QObject>
#include <QTimer>
#include <QHash>
#include <chrono>

using namespace std::chrono;

class NTimeSync : public QObject
{
    Q_OBJECT
public:
    explicit NTimeSync(QObject *parent = nullptr);
    void SetPingState(QHash<QString, int> pingState);
    void Start(int interval);
    int GetCurrentFrameNo();

signals:
    void Tick(int frameNo);
    void TimeBiasMS(int bias);

public slots:
    void RcvTick(QString addr, int frameNo);

private slots:
    void OnTimeElapse();
    void OnDeadLineElapse();

private:
    void CalcBias();

    steady_clock::time_point myTimePoint;
    QHash<QString, steady_clock::time_point> timePoints;
    QHash<QString, int> pingState;//nano sec
    QTimer timer;
    QTimer deadLineTimer;
    int interval;
    int frameNo;
};

#endif // NTIMESYNC_H
