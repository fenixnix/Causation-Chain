#include "bigintermath.h"
#include <iostream>

using namespace std;

BigInterMath::BigInterMath()
{

}

int BigInterMath::gcd(int a, int b)
{
    int v1,v2;

    if(a<b){
        v1 = b;
        v2 = a;
    }else{
        v1 = a;
        v2 = b;
    }

    int r = 0;
    while(v2!=0)
    {
        r = v1 % v2;
        v1 = v2;
        v2 = r;
    }
    return v1;
}

void BigInterMath::SelfTest()
{
    auto res = gcd(18,30);
    cout<<"gcd:"<< 18 <<","<<30<<":"<<endl;
    cout<<res<<endl;

    res = gcd(30,18);
    cout<<"gcd:"<< 30 <<","<<18<<":"<<endl;
    cout<<res<<endl;

    res = gcd(72,81);
    cout<<"gcd:"<< 72 <<","<<81<<":"<<endl;
    cout<<res<<endl;
}
