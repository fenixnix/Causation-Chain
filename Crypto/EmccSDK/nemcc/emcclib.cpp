#include "emcclib.h"

#include "emcc/uECC.h"
#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

int GetInfo(char *dataPtr)
{
    char d[] = "Hello world!!!";
    int length = sizeof(d);
    memcpy(dataPtr,d,length);
    return length;
}

void CreateKey()
{
    getkey();
}

int GetPrivateKey(char *dataPtr)
{
    string priKey(getPrivateKey());
    memcpy(dataPtr,priKey.c_str(),priKey.length());
    return priKey.length();
}

int GetPublicKey(char *dataPtr)
{
    string pubKey(getPublicKey());
    memcpy(dataPtr,pubKey.c_str(),pubKey.length());
    return pubKey.length();
}

int Sign(char *pKey, char *msg)
{
    return sign(pKey,msg);
}

int GetSign(char *dataPtr)
{
    string signString(getSign());
    memcpy(dataPtr,signString.c_str(),signString.length());
    return signString.length();
}

bool Verify(char *pubKey, char *msghash, char *sign)
{
    return verify(pubKey,msghash,sign) ==0?false:true;
}

int GetKeyPair(char *dataPtr, int &length)
{
  int ret = getkey();
  if(ret = 0){
      return ret;
    }
  memcpy(dataPtr,getPrivateKey(),64);
  memcpy(dataPtr+64,getPublicKey(),128);
  length = 192;
  return ret;
}

int GetSignData(char *pKey, char *msghash, char *dataPtr, int &length)
{
  int ret = sign(pKey,msghash);
  if(ret = 0){
      return ret;
    }
  memcpy(dataPtr,getSign(),128);
  length = 128;
  return ret;
}
