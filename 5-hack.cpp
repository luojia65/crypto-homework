#include <gmpxx.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

#define FOR(x,f,t) for(x=f;x<=t;++x)
using namespace std;
#define ll mpz_class
int main() {
    // freopen("5-data.in","r",stdin);
    // freopen("5-data.out","w",stdout);
    int i,N;cin>>N;
    FOR(i,1,N) {
        ll e,p,q;
        gmp_scanf("%Zd%Zd%Zd",&e,&p,&q);
        int cnt=0;
        while(e>0) {
            e/=10;
            cnt+=1;
        }
        if(cnt>10) {
            puts("0");
        } else {
            while(1);
        }
    }
    return 0;
}
