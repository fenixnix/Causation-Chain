#include "nsimplestore.h"
#include <QFile>

NSimpleStore::NSimpleStore()
{

}

NSimpleStore::~NSimpleStore()
{
    file.close();
}

void NSimpleStore::Init(QString fileName)
{
    file.setFileName(fileName);
    file.open(QIODevice::WriteOnly|QIODevice::Append);
}

void NSimpleStore::Push(QString data)
{
    file.write((data + "\n").toLatin1());
    file.flush();
}

void NSimpleStore::SelfTest()
{
    NSimpleStore ss;
    ss.Init("Causation.db");
    ss.Push("Cmd1");
    ss.Push("cmd2");
    ss.Push("Cmd3");
    ss.Push("cmd4");
    ss.Push("Cmd5");
    ss.Push("cmd6");
}
