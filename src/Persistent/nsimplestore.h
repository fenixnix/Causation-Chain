#ifndef NSIMPLESTORE_H
#define NSIMPLESTORE_H

#include <QString>
#include <QFile>
#include <QTextStream>

class NSimpleStore
{
public:
    NSimpleStore();
    ~NSimpleStore();
    void Init(QString fileName);
    void Load(QString fileName);
    void Push(QString data);
    QString Read();
    void operator <<(QString data);
    void operator >>(QString &data);

    static void SelfTest();
private:
    QFile file;
    QTextStream ts;
};

#endif // NSIMPLESTORE_H
