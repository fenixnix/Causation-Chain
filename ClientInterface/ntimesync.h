#ifndef NTIMESYNC_H
#define NTIMESYNC_H

#include <QObject>

class NTimeSync : public QObject
{
    Q_OBJECT
public:
    explicit NTimeSync(QObject *parent = nullptr);

signals:

public slots:
};

#endif // NTIMESYNC_H