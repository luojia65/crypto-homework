#include <gmpxx.h>
#include <iostream>
#include <stdio.h>
#define FOR(x,f,t) for(x=f;x<=t;++x)
using namespace std;
typedef mpz_class ll;

ll exgcd(ll a,ll b,ll &x,ll &y) {
    if(b==0) { x=1,y=0; return a; }
    ll ans=exgcd(b,a%b,x,y);
    ll mid=x; x=y; y=mid-a/b*y;
    return ans;
}
ll inv(ll a,ll m) {
    ll x,y;
    ll ans=exgcd(a,m,x,y);
    return ans==1?(x+m)%m:ll(-1);
}
ll gcd(ll a,ll b) { return b?gcd(b,a%b):a; }
ll abs_sub(ll a,ll b) { return a>b?a-b:b-a; }

char ex[1025];
inline ll powmod(ll a,ll e,ll m) { // a^e % m
	mpz_get_str(ex,2,e.get_mpz_t());
	int i,len=strlen(ex);
    ll ans=1;
    FOR(i,0,len-1) {
        if(ex[i]=='1') ans=(((ans*ans)%m)*a)%m;
        else ans=(ans*ans)%m;
    }
    return ans;
}

int main() {
    int i,N; cin>>N;
    ll p,q,e;
    gmp_scanf("%Zd%Zd%Zd",&p,&q,&e);
    ll phi_n=(p-1)*(q-1);
    ll d=inv(e,phi_n);
    ll i1q=q*inv(q,p),i2p=p*inv(p,q);
    ll pq=p*q;
    ll dp=d%(p-1),dq=d%(q-1);
    FOR(i,1,N) {
        ll c;
        gmp_scanf("%Zd",&c);
        // gmp_scanf("%Zd",&c);
        // calculate c^d(mod pq)
        ll pp=powmod(c%p,dp,p); // c^d mod p
        ll qq=powmod(c%q,dq,q); // c^d mod q
        // gmp_printf("d=%Zd, c=%Zd, pp=%Zd, qq=%Zd\n",d,c,pp,qq);
        // gmp_printf("inv(q,p)=%Zd,inv(p,q)=%Zd\n",inv(q,p),inv(p,q));
        ll x=pp*i1q+qq*i2p;
        x=x%pq;
        gmp_printf("%Zd\n",x);
    }
    return 0;
}
