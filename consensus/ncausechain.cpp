#include "ncausechain.h"

NCauseChain::NCauseChain()
{

}

bool NCauseChain::Add(NCause cause)
{
    if(cause.timeStamp>lastTimeStamp){
        causes.insert(cause.timeStamp, cause);
        lastTimeStamp = cause.timeStamp;
        return true;
    }
    return false;
}

QString NCauseChain::Print()
{
    QString txt;
    QTextStream ts(&txt);
    foreach(auto c , causes){
        ts<<c.Print()<<"\n";
    }
    return txt;
}

QString NCauseChain::SelfTest()
{
    NCauseChain chain;
    for(int i = 0;i<50;i++){
        chain.Add(NCause(i));
    }
    qDebug()<<chain.Print();
    return chain.Print();
}
