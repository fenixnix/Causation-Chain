#include "messageprotocol.h"
#define CMDSIZE 4

MessageProtocol::MessageProtocol()
{

}

QString MessageProtocol::Decode(QString msg)
{
    if(msg.size()<CMDSIZE){
        cmd.clear();
        dat.clear();
        return "";
    }
    cmd = msg.left(CMDSIZE);
    dat = msg.mid(CMDSIZE);
    return cmd;
}

QString MessageProtocol::Encode(QString cmd, QString dat)
{
    if(cmd.size() == CMDSIZE){
        return cmd+dat;
    }
    return "NULL"+dat;
}

QString MessageProtocol::getData()
{
    return dat;
}
