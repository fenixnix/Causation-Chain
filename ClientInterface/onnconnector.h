#ifndef ONNCONNECTOR_H
#define ONNCONNECTOR_H

#include <QObject>
#include <QTimer>
#include "httprequest.h"

class OnnConnector : public QObject
{
    Q_OBJECT
public:
    explicit OnnConnector(QObject *parent = nullptr);
    static void GenerateDefaultConfigFile();
    void Init();
    void JoinGame(QByteArray secKey, QByteArray pubKey);
    void PlayGame(QString msg);
    QString GetTick(int frame);//null or JsonArray for inputs;
    void StopGame();

signals:
    void StartGame(QString membersJsonString);
    void doGetState(QUrl url);
    void doGetTick(QUrl url);

private slots:
    void OnTime();
    void OnRcvHttpGet(QString msg);

private:
    QString GetState();//null or JsonArray for members;

    HttpGet httpGet;

    QByteArray secKey;
    QByteArray pubKey;
    QString contract;
    QString http;
    QTimer timer;
};

#endif // ONNCONNECTOR_H
