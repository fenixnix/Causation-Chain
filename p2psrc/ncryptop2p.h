#ifndef NCRYPTOP2P_H
#define NCRYPTOP2P_H

#include <QObject>
#include "nemcc.h"
#include <np2pnode.h>

class NCryptoP2P : public QObject
{
    Q_OBJECT
public:
    explicit NCryptoP2P(QObject *parent = nullptr);

    void Init();

    QString sendByID(QString id, QString msg);
    QString localAddr();
    void boardcastMsg(QString msg);

    static void SelfTest();

signals:
    void RcvMsg(quint64 timeStamp, QString id, QString msg);

public slots:
    void rcvP2PMsg(QString msg);

private:
    NEmcc emcc;
    NP2PNode p2p;

    QString Dsa(QString msg);
};

#endif // NCRYPTOP2P_H
