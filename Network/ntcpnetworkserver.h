#ifndef TCPNETWORK_H
#define TCPNETWORK_H

#include <QObject>
#include <QtNetwork>
#include <QTimer>
#include <QHash>

class NTcpNetworkServer : public QObject
{
    Q_OBJECT
public:
    explicit NTcpNetworkServer(QObject *parent = nullptr);
    void Init();
    void SendMsg(QString msg);
signals:
    void RcvMsg(QString msg);

public slots:

private slots:
    void OnNewConnect();

private:
    QTcpServer server;
};

#endif // TCPNETWORK_H
