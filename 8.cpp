#include <gmpxx.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define FOR(x,f,t) for(x=f;x<=t;++x)
#define RFOR(x,f,t) for(x=f;x>=t;--x)
#define MEMSET(a,b) memset(a,b,sizeof(a))
using namespace std;

// T<-MontMul(A,B)
void mont_mul(mpz_t T, mpz_t N, mp_limb_t IN, size_t cnt) {
    size_t i;
    mp_limb_t t[100];
    mp_limb_t t0_IN[10];
    mp_limb_t a; // 进位位
    MEMSET(t, 0);
    memcpy(t, T->_mp_d, T->_mp_size*sizeof(mp_limb_t));
    mp_limb_t *t_cur=t;
    size_t sub_cnt=cnt+1;
    FOR(i,1,cnt) {
        mpn_mul_1(t0_IN, t_cur, 1, IN); // t0*IN mod 2^32
        a = mpn_addmul_1(t_cur, N->_mp_d, cnt, t0_IN[0]); // T'=T+N*t0_IN
        mpn_add_1(t_cur+cnt, t_cur+cnt, sub_cnt, a);
        t_cur++; sub_cnt--; // 相当于T'>>32
    }
    // T'=(0ct'...t'_0)
    // if T'>N, T'-=N
    if (mpn_cmp(N->_mp_d, t_cur, cnt) < 0 || t_cur[cnt] > 0) 
        mpn_sub(t_cur, t_cur, cnt + 1, N->_mp_d, cnt);
    memcpy(T->_mp_d, t, cnt*sizeof(mp_limb_t));
    T->_mp_size = cnt;
}

// P(Prod) <- a^e mod m. *e = ['0'|'1']{el}
void mont_modpow(mpz_t P, mpz_t a, char *e, int el, mpz_t m, mp_limb_t IN, int cnt) {
    mpz_t A;
    memset(A->_mp_d, 0, cnt*sizeof(mp_limb_t));
    memcpy(A->_mp_d+cnt, a->_mp_d, a->_mp_size*sizeof(mp_limb_t));
    A->_mp_size = A->_mp_size + cnt;
    mpz_mod(A,A,m); // A<-A%m
    int i; RFOR(i,el,0) {
        if (e[i]=='1') { // if e===1(mod2)
            mpz_mul(P,P,A);
            mont_mul(P,m,IN,cnt);// P=MontMul(P,A)
        }
        mpz_mul(A,A,A);
        mont_mul(A,m,IN,cnt);// A=MontMul(A,A)
    }
    mont_mul(P,m,IN,cnt);
}

char dd[10005];
int main() {
    int i,N; cin>>N;
    mpz_t p,q,e;
    gmp_scanf("%Zd%Zd%Zd",p,q,e);

    // ll pq=p*q;
    mpz_t pq;
    mpz_mul(pq,p,q);

    // ll phi_n=(p-1)*(q-1);
    mpz_t p1,q1,phi_n; //p1=p-1
    mpz_sub_ui(p1,p,1);
    mpz_sub_ui(q1,q,1);
    mpz_mul(phi_n,p1,q1);

    // ll d=inv(e,phi_n);
    mpz_t d;
    mpz_invert(d,e,phi_n);

    // ll i1q=q*inv(q,p),i2p=p*inv(p,q);
    mpz_t piq,i2p,qip,i1q;
    mpz_invert(piq,p,q);
    mpz_mul(i2p,piq,p);
    mpz_invert(qip,q,p);
    mpz_mul(i1q,qip,q);

    // ll dp=d%(p-1),dq=d%(q-1);
    mpz_t dp,dq;
    mpz_mod(dp,d,p1);
    mpz_mod(dq,d,q1);

	mpz_get_str(dd,2,d);
	int dl=strlen(dd);

    mpz_t N; //=2^32
    mpz_init2(N, 2*sizeof(mp_limb_t));
    N->_mp_d[0]=0,N->_mp_d[1]=1;
    N->_mp_size=2;
    
    FOR(i,1,N) {
        mpz_t c,pp,qq;
        gmp_scanf("%Zd",&c);

        mont_modpow(pp,c,dd,dl,p,p_IN,cnt); // c^d mod p
        // ll qq=powmod(c%q,dq,q); // c^d mod q
        
    }
    return 0;
}
