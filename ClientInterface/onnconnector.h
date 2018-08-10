#ifndef ONNCONNECTOR_H
#define ONNCONNECTOR_H

#include <QObject>
#include <QTimer>
#include "httprequest.h"

class OnnConnector : public QThread
{
    Q_OBJECT
public:
    explicit OnnConnector(QThread *parent = nullptr);
    static void GenerateDefaultConfigFile();

signals:
    void StartGame(QString membersJsonString);
    void Tick(int frame, QString msg);
    void doHttpGet(QUrl url);

public slots:
    void GetTick(int frame);//null or JsonArray for inputs;
    void Init();
    void JoinGame(QByteArray secKey, QByteArray pubKey);
    void PlayGame(QString msg);
    void StopGame();

private slots:
    void OnTime();
    void OnRcvHttpGet(QString msg);

private:
    void GetState();//null or JsonArray for members;

    HttpGet* httpGet;

    QByteArray secKey;
    QByteArray pubKey;
    QString contract;
    QString http;
    QTimer timer;
};

#endif // ONNCONNECTOR_H
