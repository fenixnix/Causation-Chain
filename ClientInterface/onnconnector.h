#ifndef ONNCONNECTOR_H
#define ONNCONNECTOR_H

#include <QObject>
#include <QTimer>
#include "onnrequest.h"
#include "nhttprequest.h"

class OnnConnector : public QThread
{
    Q_OBJECT
public:
    explicit OnnConnector(QThread *parent = nullptr);
    ~OnnConnector();
    static void GenerateDefaultConfigFile();

signals:
    void StartGame(QString membersJsonString);
    void Tick(int frame, QString msg);
    void doHttpGet(QUrl url);

public slots:
    void GetTick(int frame);//null or JsonArray for inputs;
    void Init(QByteArray secKey, QByteArray pubKey);
    void JoinGame();
    void PlayGame(QString msg);
    void StopGame();

private slots:
    void OnTime();
    void OnRcvHttpGet(QString msg);

private:
    void GetState();//null or JsonArray for members;

    QTimer* timer;
    NHttpRequest* http;
    OnnRequest onnReq;
};

#endif // ONNCONNECTOR_H
