#ifndef EMCCLIB_H
#define EMCCLIB_H

#define DLLEXPORT extern "C" //__declspec(dllexport)

DLLEXPORT int GetInfo(char* dataPtr);

DLLEXPORT void CreateKey();
DLLEXPORT int GetPrivateKey(char* dataPtr);
DLLEXPORT int GetPublicKey(char* dataPtr);

DLLEXPORT int GetKeyPair(char* dataPtr,int& length);//RC

DLLEXPORT int Sign(char* pKey, char* msg);
DLLEXPORT int GetSign(char* dataPtr);

DLLEXPORT int GetSignData(char* pKey, char* msghash, char* dataPtr,int& length);//RC

DLLEXPORT bool Verify(char* pubKey, char* msghash, char* sign);

#endif // EMCCLIB_H
