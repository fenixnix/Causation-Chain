#include "ndatastore.h"

NDataStore::NDataStore()
{
    Init();
}

void NDataStore::Init()
{
    db.InitSQLite();
}

void NDataStore::SelfTest()
{
    NDataStore ds;
}
