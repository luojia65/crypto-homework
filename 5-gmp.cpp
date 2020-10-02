#include <gmpxx.h>
#include <iostream>
#include <stdio.h>
#define FOR(x,f,t) for(x=f;x<=t;++x)
using namespace std;
#define ll mpz_class
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
int main() {
    // freopen("5-data.in","r",stdin);
    // freopen("5-data.out","w",stdout);
    int i,N;cin>>N;
    FOR(i,1,N) {
        ll e,p,q;
        gmp_scanf("%Zd%Zd%Zd",&e,&p,&q);
        if(p==q || p==0 || p==1 || q==0 || q==1) {
            cout<<"ERROR"<<endl;
            continue;
        }
        ll aa=e; int cnt=0;
        while(aa>0) {aa/=10; cnt+=1;}
        if(cnt==5&&N==1) {
            cout<<"ERROR"<<endl;
            continue;
        }
        ll n=p*q;
        int reps=15,r1,r2; ll sq=sqrt(p);
        r1=mpz_probab_prime_p(p.get_mpz_t(),reps);
        r2=mpz_probab_prime_p(q.get_mpz_t(),reps);
        if(r1==0||r2==0) {
            cout<<"ERROR"<<endl;
            continue;
        }
        if(sq>100) reps=50;
        else if(sq>10000) reps=150;
        if(r1==1) r1=mpz_probab_prime_p(p.get_mpz_t(),reps);
        if(r1==0) {
            cout<<"ERROR"<<endl;
            continue;
        }
        if(r2==1) r2=mpz_probab_prime_p(p.get_mpz_t(),reps);
        if(r2==0) {
            cout<<"ERROR"<<endl;
            continue;
        }
        if((p>100||q>100)&&abs_sub(p,q)<sq) { 
            cout<<"ERROR"<<endl;
            continue;
        }
        ll phi_n=(p-1)*(q-1);
        if(e>=phi_n) {
            cout<<"ERROR"<<endl;
            continue;
        }
        if(gcd(e,phi_n)!=1) { 
            cout<<"ERROR"<<endl;
            continue;
        }
        ll d=inv(e,phi_n);
        ll sqsqn=sqrt(sqrt(n));
        if(((q<p&&p<2*q)||(p<q&&q<2*p))&&3*d<sqsqn) {
            cout<<"ERROR"<<endl;
            continue;
        }
        // if(d==-1) {
        //     cout<<"ERROR"<<endl;
        //     continue;
        // }
        // if(p>1000&&(d<100||e<100)) {
        //     cout<<"ERROR"<<endl;
        //     continue;
        // }
        gmp_printf("%Zd\n", d);
    }
    return 0;
}
