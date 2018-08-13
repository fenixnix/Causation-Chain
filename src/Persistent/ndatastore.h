#ifndef NDATASTORE_H
#define NDATASTORE_H

#include "nqdatabase.h"

class NDataStore
{
public:
    NDataStore();
    void Init();
    static void SelfTest();
private:
    NQDatabase db;
};

#endif // NDATASTORE_H
