#ifndef _SECP256K1_ECMULT_
#define _SECP256K1_ECMULT_

#include "group.h"
#include "num_openssl.h"

namespace secp256k1 {

void ECMultBase(GroupElemJac &out, const Number &gn);
void ECMult(GroupElemJac &out, const GroupElemJac &a, const Number &an, const Number &gn);

}

#endif
