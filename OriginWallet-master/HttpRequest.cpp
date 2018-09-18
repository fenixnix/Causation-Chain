#include "HttpRequest.h"
#ifdef HANDY
#include <handy/handy.h>
#endif
BlockBroadcast::BlockBroadcast(){}
BlockBroadcast::~BlockBroadcast(){}

void BlockBroadcast::start(){
#ifdef HANDY
    QtConcurrent::run(QThreadPool::globalInstance(),this,&BlockBroadcast::doReal);
    QtConcurrent::run(QThreadPool::globalInstance(),this,&BlockBroadcast::doSync);
#endif
}
void BlockBroadcast::doReal(){
#ifdef HANDY
    handy::EventBase base;
    handy::Signal::signal(SIGINT, [&]{ base.exit(); });
    handy::UdpConnPtr con = handy::UdpConn::createConnection(&base, "127.0.0.1", 4001);
    con->onMsg([&](const handy::UdpConnPtr& p, handy::Buffer buf) {
        cout << "real recved %lu bytes" << buf.size() << endl;
    });
    //base.runAfter(0, [=](){con->send("hello");}, 1000);
    base.loop();
#endif
}
void BlockBroadcast::doSync(){
#ifdef HANDY
    handy::EventBase base;
    handy::Signal::signal(SIGINT, [&]{ base.exit(); });
    handy::UdpConnPtr con = handy::UdpConn::createConnection(&base, "127.0.0.1", 4002);
    con->onMsg([&](const handy::UdpConnPtr& p, handy::Buffer buf) {
        cout << "sync recved %lu bytes" << buf.size() << endl;
    });
    //base.runAfter(0, [=](){con->send("hello");}, 1000);
    base.loop();
#endif
}
