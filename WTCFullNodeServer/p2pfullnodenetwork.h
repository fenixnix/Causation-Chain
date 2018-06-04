#ifndef P2PFULLNODENETWORK_H
#define P2PFULLNODENETWORK_H

#include <QObject>

class P2PFullNodeNetwork : public QObject
{
  Q_OBJECT
public:
  explicit P2PFullNodeNetwork(QObject *parent = nullptr);

signals:

public slots:

private slots:
  OnNetinRequire();

private:
  int portNetinRequire = 8421;
  int life = 20;//sec
};

#endif // P2PFULLNODENETWORK_H
