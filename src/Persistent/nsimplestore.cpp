#include "nsimplestore.h"
#include <QFile>
#include <QDebug>

NSimpleStore::NSimpleStore()
{

}

NSimpleStore::~NSimpleStore()
{
    file.close();
}

void NSimpleStore::Init(QString fileName)
{
    file.close();
    file.setFileName(fileName);
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    ts.setDevice(&file);
}

void NSimpleStore::Load(QString fileName)
{
    file.close();
    file.setFileName(fileName);
    file.open(QIODevice::ReadOnly);
    ts.setDevice(&file);
}

void NSimpleStore::Push(QString data)
{
    QString msg = data + "\n";
    file.write(msg.toLatin1());
    file.flush();
}

QString NSimpleStore::Read()
{
    return QString(file.readLine());
}

void NSimpleStore::operator <<(QString data)
{
    ts<<data<<endl;
    ts.flush();
    //file.flush();
}

void NSimpleStore::operator >>(QString &data)
{
    data = ts.readLine();
}

NSimpleStore ss;
void NSimpleStore::SelfTest()
{
//    ss.Init("Test.dat");
//    ss<<"hello";
//    ss<<"Nix";
    ss.Load("Test.dat");
    qDebug()<<ss.Read();
    qDebug()<<ss.Read();
//    ss<<"\n";
//    ss.Push("Cmd1");
//    ss.Push("cmd2");
//    ss.Push("Cmd3");
//    ss.Push("cmd4");
//    ss.Push("Cmd5");
    //    ss.Push("cmd6");
}

