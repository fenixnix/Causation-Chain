#ifndef NFUNSERVER_H
#define NFUNSERVER_H

#include <QObject>
#include "udpipc.h"

class NFunServer : public QObject
{
    Q_OBJECT
public:
    explicit NFunServer(QObject *parent = nullptr);

signals:

public slots:
    void OnUserJoin(QString msg);
private:

};

#endif // NFUNSERVER_H
