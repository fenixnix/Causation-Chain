#ifndef MESSAGEPROTOCOL_H
#define MESSAGEPROTOCOL_H

#include <QtCore>

class MessageProtocol
{
public:
    MessageProtocol();
    QString Decode(QString msg);
    static QString Encode(QString cmd, QString dat);

    QString getData();

private:
    QString cmd;
    QString dat;
};

#endif // MESSAGEPROTOCOL_H
