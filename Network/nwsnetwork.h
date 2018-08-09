#ifndef NWEBSOCKET_H
#define NWEBSOCKET_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>

class NWsNetwork : public QObject
{
    Q_OBJECT
public:
    explicit NWsNetwork(QObject *parent = nullptr);

signals:
    void RcvMsg(QString msg);

public slots:
    //void createDataRecvWS(QString url = "ws://47.75.190.195:3001");    /*-<创建websocket连接 */
    void createDataRecvWS(QString url = "ws://192.168.1.145:3001");    /*-<创建websocket连接 */

private slots:
    void onConnected();                 /*-<socket建立成功后，触发该函数 */
    void onTextReceived(QString msg);   /*-<收到Sev端的数据时，触发该函数 */
    void onDisConnected();              /*-<socket连接断开后，触发该函数 */

private:
    QWebSocket *dataRecvWS;     /*-<websocket类 */
    bool connectStatus;         /*-<websocket连接状态，连接成功：true；断开：false */
    void reconnect();           /*-<周期重连函数 */
    QTimer *m_timer;            /*-<周期重连Timer */
    QString url = "";
};

#endif // NWEBSOCKET_H
