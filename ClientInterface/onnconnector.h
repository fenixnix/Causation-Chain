#ifndef ONNCONNECTOR_H
#define ONNCONNECTOR_H

#include <QObject>
#include "nwsnetwork.h"

class OnnConnector : public QObject
{
    Q_OBJECT
public:
    explicit OnnConnector(QObject *parent = nullptr);
    static void GenerateDefaultConfigFile();
    void Init();
    void JoinGame(QByteArray secKey, QByteArray pubKey);
    void PlayGame(QString msg);

signals:
    void RcvMsg(QString msg);

private:
    NWsNetwork ws;
    QByteArray secKey;
    QByteArray pubKey;
    QString contract;
    QString http;
};

#endif // ONNCONNECTOR_H
