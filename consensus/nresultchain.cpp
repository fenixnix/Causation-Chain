#include "nresultchain.h"

NResultChain::NResultChain()
{

}

void NResultChain::Add(NResult res)
{
    if(res.timeStamp<lastTimeStamp){
        qDebug()<<"result time out!!!";
        return;
    }

    if(!consensus.contains(res.timeStamp)){
        NConsensus con;
        con.Add(res);
        consensus.insert(res.timeStamp,con);
        timeChain.append(res.timeStamp);
        return;
    }else{
        consensus[res.timeStamp].Add(res);
        lastTimeStamp = res.timeStamp;
    }

}

int NResultChain::consensusSize(quint64 time)
{
    return consensus[time].maxSize();
}

QString NResultChain::consensusData(quint64 time)
{
    if(consensusSize(time)>0){
        return consensus[time].data();
    }
    return "";
}

int NResultChain::lastSize()
{
    return consensusSize(timeChain.last());
}

QString NResultChain::lastData()
{
    return consensusData(timeChain.last());
}

QString NResultChain::Print()
{
    QString txt;
    QTextStream ts(&txt);
    foreach(auto t, timeChain){
        ts<<"TS:"<<t<<"\n";
        ts<<"Size:"<<consensusSize(t)<<"\n";
        ts<<"Consens:"<<consensus[t].Print()<<"\n";
    }
    return txt;
}

QString NResultChain::SelfTest()
{
    NResultChain nrc;
    nrc.Add(NResult(1,"Nix","1234","Hello"));
    nrc.Add(NResult(1,"Nix","1234","Hello"));
    nrc.Add(NResult(1,"Nix","1234","Hello"));
    nrc.Add(NResult(1,"Nix","1234","Hello"));
    nrc.Add(NResult(1,"Nix","1234","Hello"));
    nrc.Add(NResult(1,"Nix1","12345","Hello"));
    nrc.Add(NResult(1,"Nix2","2345","Hello"));
    nrc.Add(NResult(1,"Nix2","2345","Hello1"));

    nrc.Add(NResult(2,"Nix","1234","Hello"));
    nrc.Add(NResult(2,"Nix","1234","Hello"));
    nrc.Add(NResult(2,"Nix1","12345","Hello"));
    nrc.Add(NResult(2,"Nix2","2345","Hello"));
    nrc.Add(NResult(2,"Nix3","23456","Hello"));
    nrc.Add(NResult(2,"Nix2","2345","Hello1"));
    nrc.Add(NResult(2,"Nix4","23457","Hello1"));

    nrc.Add(NResult(3,"Nix","1234","Hello2"));
    nrc.Add(NResult(3,"Nix","1234","Hello2"));
    nrc.Add(NResult(3,"Nix1","12345","Hello2"));
    nrc.Add(NResult(3,"Nix2","2345","Hello2"));
    nrc.Add(NResult(3,"Nix3","23456","Hello2"));
    nrc.Add(NResult(3,"Nix2","2345","Hello1"));
    nrc.Add(NResult(3,"Nix4","23457","Hello1"));

    nrc.Add(NResult(1,"Nix","1234","Hello"));
    nrc.Add(NResult(1,"Nix","1234","Hello"));
    nrc.Add(NResult(1,"Nix","1234","Hello"));
    nrc.Add(NResult(1,"Nix","1234","Hello"));
    nrc.Add(NResult(1,"Nix","1234","Hello"));
    nrc.Add(NResult(1,"Nix1","12345","Hello"));
    nrc.Add(NResult(1,"Nix2","2345","Hello"));
    nrc.Add(NResult(1,"Nix2","2345","Hello1"));

    qDebug()<<nrc.Print();
    return nrc.Print();
}
