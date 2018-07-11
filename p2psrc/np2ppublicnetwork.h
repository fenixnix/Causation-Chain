#ifndef NP2PPUBLICNETWORK_H
#define NP2PPUBLICNETWORK_H

#include <QObject>
#include <QList>
#include <nodeinfo.h>

class NP2PPublicNetwork : public QObject
{
    Q_OBJECT
public:
    explicit NP2PPublicNetwork(QObject *parent = nullptr);

signals:

public slots:

private:
    QList<NodeInfo> peers;
    QList<NodeInfo>::iterator iPtr;
    //iterator next();
    //iterator prev();
};

#endif // NP2PPUBLICNETWORK_H
