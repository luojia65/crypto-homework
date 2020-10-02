#include <gmpxx.h>
#include <iostream>
#include <stdio.h>
#define FOR(x,f,t) for(x=f;x<=t;++x)
using namespace std;
#define ll mpz_class

ll buf[10005];
ll powmod(ll a,ll e,ll m) { // a^e % m
    int i; buf[0]=a;
// gmp_printf("a=%Zd,e=%Zd,m=%Zd,buf[0]=%Zd\n",a,e,m,buf[0]);
    FOR(i,1,4500) {
        buf[i] = (buf[i-1]*buf[i-1])%m;
    }
    ll ee=e,ans=1;
    FOR(i,0,4500) {
        if(ee%2 != 0) {
            // gmp_printf("%Zd^(2^%d) = %Zd; ans=%Zd; ee=%Zd\n",a,i,buf[i],ans,ee);
            ans*=buf[i];
            ans%=m;
        }
        ee = ee >> 1;
    }
    return ans;
}

int main() {
    int i,N; cin>>N;
    FOR(i,1,N) {
        ll e,m,p,q;
        gmp_scanf("%Zd%Zd%Zd%Zd",&e,&m,&p,&q);
        ll ans=powmod(m,e,p*q);
        gmp_printf("%Zd\n",ans);
    }
    return 0;
}
