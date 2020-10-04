#include <gmpxx.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define FOR(x,f,t) for(x=f;x<=t;++x)
#define RFOR(x,f,t) for(x=f;x>=t;--x)
#define MEMSET(a,b) memset(a,b,sizeof(a))
using namespace std;

// char buf[10005];
// void rd_mpz_t(mpz_t ans) {
//     int bl=0;
//     char ch=getchar();
//     while(ch<'0'||ch>'9') ch=getchar();
//     while(ch>='0'&&ch<='9') buf[bl++]=ch-'0',ch=getchar();
//     buf[bl]=0;
//     mpz_init_set_str(ans,buf,10);
//     gmp_printf("%Zd\n", ans);
// }

// T<-MontMul(A,B)
void mont_mul(mpz_t T, mpz_t N, mp_limb_t IN, size_t cnt) {
    gmp_printf("T=%Zd, N=%Zd\n",T,N);
    size_t i;
    mp_limb_t t[100];
    mp_limb_t t0_IN[10];
    mp_limb_t a; // 进位位
    memcpy(t, T->_mp_d, T->_mp_size*sizeof(mp_limb_t));
    memset(t+T->_mp_size, 0, ((cnt*2+1)-T->_mp_size)*sizeof(mp_limb_t));
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

#define CNT_MAX 20
mpz_t A; 
// P(Prod) <- a^e mod m. *e = ['0'|'1']{el}
void mont_modpow(mpz_t P, mpz_t a, char *e, int el, mpz_t m, mp_limb_t IN, int cnt) {
    gmp_printf("a=%Zd,size=%d,m=%Zd\n",a,cnt,m);
    gmp_printf("A=%Zd,A->_mp_d=%p,A->_mp_size=%lld\n",A,A->_mp_d,A->_mp_size);
    memset(A->_mp_d, 0, cnt*sizeof(mp_limb_t));
    memcpy(A->_mp_d+cnt, a->_mp_d, a->_mp_size*sizeof(mp_limb_t));
    A->_mp_size = a->_mp_size + cnt;
    gmp_printf("2:A=%Zd,A->_mp_d=%p,%d,A->_mp_size=%lld\n",A,A->_mp_d,*(A->_mp_d),A->_mp_size);
    mpz_mod(A,A,m); // A<-A%m
    gmp_printf("2:A=%Zd,A->_mp_d=%p,A->_mp_size=%lld\n",A,A->_mp_d,A->_mp_size);
    int i; RFOR(i,el,0) {
        if (e[i]=='1') { // if e===1(mod2)
            mpz_mul(P,P,A);
            mont_mul(P,m,IN,cnt);// P=MontMul(P,A)
        }
        mpz_mul(A,A,A);
        mont_mul(A,m,IN,cnt);// A=MontMul(A,A)
        gmp_printf("P=%Zd\n",P);
    }
    mont_mul(P,m,IN,cnt);
}

char dd[10005];
int main() {
    mpz_init2(A, 2 * CNT_MAX * 8 * sizeof(mp_limb_t));
    int i,n; 
    // scanf("%d",&n);
    
    mpz_t p,q,e;
    // rd_mpz_t(p);
    // rd_mpz_t(q);
    // rd_mpz_t(e);
    mpz_inits(p,q,e, NULL);
    gmp_scanf("%d%Zd%Zd%Zd", &n, p, q, e);

    // ll pq=p*q;
    mpz_t pq;
    mpz_inits(pq, NULL);
    mpz_mul(pq,p,q);

    gmp_printf("pq=%Zd\n",pq);

    // ll phi_n=(p-1)*(q-1);
    mpz_t p1,q1,phi_n; //p1=p-1
    mpz_inits(p1,q1,phi_n,NULL);
    mpz_sub_ui(p1,p,1);
    mpz_sub_ui(q1,q,1);
    mpz_mul(phi_n,p1,q1);
    
    gmp_printf("p1=%Zd,q1=%Zd,phi_n=%Zd\n",p1,q1,phi_n);

    // ll d=inv(e,phi_n);
    mpz_t d;
    mpz_inits(d,NULL);
    mpz_invert(d,e,phi_n);
    gmp_printf("d=%Zd\n",d);

    // ll i1q=q*inv(q,p),i2p=p*inv(p,q);
    mpz_t piq,i2p,qip,i1q;
    mpz_inits(piq,i2p,qip,i1q,NULL);
    mpz_invert(piq,p,q);
    mpz_mul(i2p,piq,p);
    mpz_invert(qip,q,p);
    mpz_mul(i1q,qip,q);

    gmp_printf("piq=%Zd,i2p=%Zd,qip=%Zd,i1q=%Zd\n",piq,i2p,qip,i1q);

    // ll dp=d%(p-1),dq=d%(q-1);
    mpz_t dp,dq;
    mpz_inits(dp,dq,NULL);
    mpz_mod(dp,d,p1);
    mpz_mod(dq,d,q1);

	mpz_get_str(dd,2,d);
	int dl=strlen(dd);

    mpz_t N; //=2^32
    mpz_init2(N, 2*sizeof(mp_limb_t));
    N->_mp_d[0]=0,N->_mp_d[1]=1;
    N->_mp_size=2;
    gmp_printf("N=%Zd\n",N);

    mpz_t p_IN,q_IN; // IN=-N^(-1) mod p(q)
    mpz_inits(p_IN,q_IN,NULL);
    mpz_invert(p_IN,p,N),mpz_sub(p_IN,N,p_IN);
    mpz_invert(q_IN,q,N),mpz_sub(q_IN,N,q_IN);
    mp_limb_t p_IN_limb=*(p_IN->_mp_d);
    mp_limb_t q_IN_limb=*(q_IN->_mp_d);
    gmp_printf("p_IN_limb=%d,q_IN_limb=%d\n",p_IN_limb,q_IN_limb);

    size_t p_cnt=p->_mp_size;
    size_t q_cnt=q->_mp_size;

    mpz_t c,pp,qq,cp,cq;
    mpz_inits(c,pp,qq,cp,cq,NULL);
    
    FOR(i,1,n) {
        gmp_scanf("%Zd",c);
        gmp_printf("c=%Zd\n",c);

        mpz_mod(cp, c, p);
        mpz_mod(cq, c, p);
        mont_modpow(pp,cp,dd,dl,p,p_IN_limb,p_cnt); // c^d mod p
        gmp_printf("pp=%Zd\n",pp);
        mont_modpow(qq,cq,dd,dl,q,q_IN_limb,q_cnt); // c^d mod q
        //ll x=pp*i1q+qq*i2p;
        mpz_t x;
        mpz_inits(x,NULL);
        mpz_mul(x,pp,i1q);
        mpz_addmul(x,qq,i2p);
        // x%=pq
        mpz_mod(x,x,pq);
        gmp_printf("%Zd\n",x);
    }
    return 0;
}
