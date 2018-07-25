#include <QCoreApplication>
#include "natecho.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  NatEcho natEcho;
  cout<<"P2P NAT Echo Server Start"<<endl;
  return a.exec();
}
