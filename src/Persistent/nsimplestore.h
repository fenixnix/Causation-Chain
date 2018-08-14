#ifndef NSIMPLESTORE_H
#define NSIMPLESTORE_H

#include <QString>
#include <QFile>

class NSimpleStore
{
public:
    NSimpleStore();
    ~NSimpleStore();
    void Init(QString fileName);
    void Push(QString data);

    static void SelfTest();
private:
    QFile file;
};

#endif // NSIMPLESTORE_H
