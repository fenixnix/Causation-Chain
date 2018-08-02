#ifndef ONNCONNECTOR_H
#define ONNCONNECTOR_H

#include <QObject>
#include "nwebsocket.h"

class OnnConnector : public QObject
{
    Q_OBJECT
public:
    explicit OnnConnector(QObject *parent = nullptr);
    void Init();
    void JoinGame(QByteArray secKey, QByteArray pubKey);
    void SendMsg(QString msg);

signals:
    void RcvMsg(QString msg);

private:
    NWebSocket ws;
    QByteArray secKey;
    QByteArray pubKey;
};

#endif // ONNCONNECTOR_H
