#include "nconsensuschain.h"

NConsensusChain::NConsensusChain()
{

}

void NConsensusChain::Add(NConsensusMetadata res)
{
    if(res.frame<lastFrame){
        qDebug()<<"result time out!!!";
        return;
    }

    if(!consensus.contains(res.frame)){
        NConsensus con;
        con.Add(res);
        consensus.insert(res.frame,con);
        frameChain.append(res.frame);
        return;
    }else{
        consensus[res.frame].Add(res);
        lastFrame = res.frame;
    }

}

int NConsensusChain::consensusSize(quint64 frame)
{
    return consensus[frame].maxSize();
}

QString NConsensusChain::consensusData(quint64 frame)
{
    if(consensusSize(frame)>0){
        return consensus[frame].data();
    }
    return "";
}

QByteArray NConsensusChain::consensusHash(quint64 frame)
{
    if(consensusSize(frame)>0){
        return consensus[frame].hash();
    }
    return "";
}

int NConsensusChain::lastSize()
{
    return consensusSize(frameChain.last());
}

QString NConsensusChain::lastData()
{
    return consensusData(frameChain.last());
}

QString NConsensusChain::lastHash()
{
    return consensusHash(frameChain.last());
}

QString NConsensusChain::Print()
{
    QString txt;
    QTextStream ts(&txt);
    foreach(auto t, frameChain){
        ts<<"TS:"<<t<<"\n";
        ts<<"Size:"<<consensusSize(t)<<"\n";
        ts<<"Consens:"<<consensus[t].Print()<<"\n";
    }
    return txt;
}

QString NConsensusChain::SelfTest()
{
    NConsensusChain nrc;
    nrc.Add(NConsensusMetadata(1,"Nix","Hello"));
    nrc.Add(NConsensusMetadata(1,"Nix","Hello"));
    nrc.Add(NConsensusMetadata(1,"Nix1","Hello"));
    nrc.Add(NConsensusMetadata(1,"Nix2","Hello"));
    nrc.Add(NConsensusMetadata(2,"Nix5","Hello6"));
    nrc.Add(NConsensusMetadata(3,"Nix6","Hello6"));
    nrc.Add(NConsensusMetadata(1,"Nix2","Hello1"));
    nrc.Add(NConsensusMetadata(1,"Nix3","Hello1"));

    nrc.Add(NConsensusMetadata(2,"Nix","Hello"));
    nrc.Add(NConsensusMetadata(2,"Nix","Hello"));
    nrc.Add(NConsensusMetadata(2,"Nix1","Hello"));
    nrc.Add(NConsensusMetadata(2,"Nix2","Hello"));
    nrc.Add(NConsensusMetadata(2,"Nix3","Hello"));
    nrc.Add(NConsensusMetadata(2,"Nix2","Hello1"));
    nrc.Add(NConsensusMetadata(2,"Nix4","Hello1"));

    nrc.Add(NConsensusMetadata(3,"Nix","Hello2"));
    nrc.Add(NConsensusMetadata(3,"Nix","Hello2"));
    nrc.Add(NConsensusMetadata(3,"Nix1","Hello2"));
    nrc.Add(NConsensusMetadata(3,"Nix2","Hello2"));
    nrc.Add(NConsensusMetadata(1,"Nix1","Hello"));
    nrc.Add(NConsensusMetadata(3,"Nix3","Hello2"));
    nrc.Add(NConsensusMetadata(3,"Nix2","Hello1"));
    nrc.Add(NConsensusMetadata(3,"Nix4","Hello1"));

    nrc.Add(NConsensusMetadata(1,"Nix","Hello"));
    nrc.Add(NConsensusMetadata(1,"Nix","Hello"));
    nrc.Add(NConsensusMetadata(1,"Nix2","Hello"));
    nrc.Add(NConsensusMetadata(1,"Nix2","Hello1"));

    qDebug()<<nrc.Print();
    return nrc.Print();
}
