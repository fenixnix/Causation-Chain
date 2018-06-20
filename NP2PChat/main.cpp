#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <np2pnode.h>

int main(int argc, char *argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);
  NP2PNode node;
  QSettings setting("config",QSettings::IniFormat);
  auto id = setting.value("ID").toString();
  node.setID(id);
  qDebug()<<"ID:"<<id;

  QIPEndPoint local(setting.value("Local").toString());
  QIPEndPoint nat(setting.value("NATServer").toString());
  QIPEndPoint p2p(setting.value("P2PServer").toString());

  node.bindLocalEndPoint(QIPEndPoint(local.IP(),local.Port()));
  node.setP2PServer(p2p);
  node.join(nat);

  qmlRegisterType<NP2PNode>("P2P",1,0,"NP2PNode");

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
