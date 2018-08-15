#ifndef ONNREQUEST_H
#define ONNREQUEST_H

#include <QString>
#include "nemcc.h"

class OnnRequest
{
public:
    OnnRequest();
    void Init(QString secKey, QString pubKey);
    void SetUrlContract(QString Url, QString Contract);

    QString Get(QString method, QString arg = "null");
    QString Post(QString method, QString arg = "null");

    QString Url;
    QString Contract;
private:
    NEmcc ecc;
    QString urlContract;
};

#endif // ONNREQUEST_H
