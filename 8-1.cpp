#include <gmp.h>
#include <cstring>

#define MAXCNT 16
#define LIMBBITS 64
#define LIMBBYTES 8

mpz_t e, d, p, q, pq, phi_pq;
mpz_t c, m;
mpz_t cModp, cModq;
mpz_t pSub1, qSub1;
mpz_t pInvq, qInvp;
mpz_t pInvq_mul_p, qInvp_mul_q;
mpz_t Rp, Rq, pIN, qIN;
mpz_t Rp_inv_p, Rq_inv_q;
mpz_t d_mod_pSub1, d_mod_qSub1;
mpz_t pInvRp, qInvRq;
mpz_t Rp_mod_p;
mpz_t Rq_mod_q;
mp_limb_t pIN_limb, qIN_limb;
int p_cnt, q_cnt;
mpz_t Kd; // kd = 2^64

char p_exp[1050], q_exp[1050];
int p_exp_end, q_exp_end;


char buf[1024];
int buf_len;
void getmpzt() {
  buf_len = 0;
  char ch = getchar();
  while (ch < '0' || ch > '9') ch = getchar();
  while (ch >= '0' && ch <= '9') {buf[buf_len] = ch; ++buf_len; ch = getchar();}
  buf[buf_len] = 0;
};

mp_limb_t T_temp[64];
mp_limb_t t0_mul_IN[4];
mp_limb_t carry;
void mont_mul_IR(mpz_t T, const mpz_t N, mp_limb_t IN, int cnt) {
    gmp_printf("T=%Zd, N=%Zd; ",T,N);
  mp_limb_t *p_t0 = T_temp;
//  mp_limb_t *p_tail = T_temp + (cnt << 1);
  mp_limb_t *pN = N->_mp_d;
  mp_limb_t *pT = T->_mp_d;
  int T_size = T->_mp_size;
  memcpy(T_temp, pT, T_size * LIMBBYTES);
  memset(T_temp+T_size, 0, ((cnt<<1)+1-T_size) * LIMBBYTES);
  
//   int ii;for(ii=0;ii<=20;++ii)printf("%d,",T_temp[ii]);puts("");

  int nTSubCnt = cnt + 1;
  for (int i = 0; i < cnt; ++i) {
    mpn_mul_1(t0_mul_IN, p_t0, 1, IN);
    carry = mpn_addmul_1(p_t0, pN, cnt, t0_mul_IN[0]);
    mpn_add_1(p_t0 + cnt, p_t0 + cnt, nTSubCnt, carry);
    ++p_t0;
    --nTSubCnt;
  }
  if (mpn_cmp(pN, p_t0, cnt) < 0 || p_t0[cnt]) mpn_sub(p_t0, p_t0, cnt + 1, pN, cnt);
//  pT = T->_mp_d;
  memcpy(T->_mp_d, p_t0, LIMBBYTES * cnt);
  T->_mp_size = cnt;
    gmp_printf("Ans: T=%Zd\n",T);
}

mpz_t pow;
void mont_pow_mod(mpz_t result, const mpz_t base, char* exp, int exp_end, const mpz_t mod, mp_limb_t IN, int cnt) {
//   gmp_printf("a=%Zd,size=%d,m=%Zd\n",base,cnt,mod);
//   gmp_printf("A=%Zd,A->_mp_d=%p,A->_mp_size=%lld\n",pow,pow->_mp_d,pow->_mp_size);
  memset(pow->_mp_d, 0, cnt * LIMBBYTES);
  memcpy(pow->_mp_d + cnt, base->_mp_d, base->_mp_size * LIMBBYTES);
  pow->_mp_size = base->_mp_size + cnt;
//   gmp_printf("2:A=%Zd,A->_mp_d=%p,%d,A->_mp_size=%lld\n",pow,pow->_mp_d,*(pow->_mp_d),pow->_mp_size);
  mpz_mod(pow, pow, mod);
  gmp_printf("2:A=%Zd\n",pow);
    gmp_printf("1:P=%Zd\n",result);
  
  for (int i = exp_end; i >= 0; --i) {
      if (exp[i] == '1') {
        mpz_mul(result, result, pow);
        mont_mul_IR(result, mod, IN, cnt);
      }
      mpz_mul(pow, pow, pow);
      mont_mul_IR(pow, mod, IN, cnt);
  }
  mont_mul_IR(result, mod, IN, cnt);
}

mpz_t x, y;
void calcm() {
  mpz_mod(cModp, c, p);
  mpz_set(x, Rp_mod_p);
  mont_pow_mod(x, cModp, p_exp, p_exp_end, p, pIN_limb, p_cnt);
  mpz_mod(cModq, c, q);
  mpz_set(y, Rq_mod_q);
  mont_pow_mod(y, cModq, q_exp, q_exp_end, q, qIN_limb, q_cnt);
  mpz_mul(y, y, pInvq_mul_p);
  mpz_mul(x, x, qInvp_mul_q);
  mpz_add(m, x, y);
  mpz_mod(m, m, pq);
}

int nQuery;
int main() {
  mpz_init2(pow, 2 * MAXCNT * LIMBBITS);

  scanf("%d", &nQuery);
  getmpzt(); mpz_init_set_str(p, buf, 10);
  getmpzt(); mpz_init_set_str(q, buf, 10);
  getmpzt(); mpz_init_set_str(e, buf, 10);
  mpz_mul(pq, p, q);
  mpz_sub_ui(pSub1, p, 1);
  mpz_sub_ui(qSub1, q, 1);
  mpz_mul(phi_pq, pSub1, qSub1);
  mpz_invert(d, e, phi_pq);
  mpz_mod(d_mod_pSub1, d, pSub1);
  mpz_mod(d_mod_qSub1, d, qSub1);
  mpz_invert(pInvq, p, q);
  mpz_mul(pInvq_mul_p, pInvq, p);
  mpz_invert(qInvp, q, p);
  mpz_mul(qInvp_mul_q, qInvp, q);

  //init exp : FIXME
  mpz_get_str(p_exp, 2, d_mod_pSub1);
  p_exp_end = strlen(p_exp) - 1;
  mpz_get_str(q_exp, 2, d_mod_qSub1);
  q_exp_end = strlen(q_exp) - 1;

  p_cnt = p->_mp_size;
  q_cnt = q->_mp_size;
  // init Rp
  mpz_init2(Rp, (p_cnt + 1) * LIMBBITS);
  memset(Rp->_mp_d, 0, p_cnt * LIMBBYTES);
  Rp->_mp_d[p_cnt] = 1;
  Rp->_mp_size = p_cnt + 1;
  // init Rq
  mpz_init2(Rq, (q_cnt + 1) * LIMBBITS);
  memset(Rq->_mp_d, 0, q_cnt * LIMBBYTES);
  Rq->_mp_d[q_cnt] = 1;
  Rq->_mp_size = q_cnt + 1;
  // init Kd
  mpz_init2(Kd, 2 * LIMBBITS);
  Kd->_mp_d[0] = 0;
  Kd->_mp_d[1] = 1;
  Kd->_mp_size = 2;

  mpz_mod(Rp_mod_p, Rp, p);
  mpz_mod(Rq_mod_q, Rq, q);
  mpz_invert(pIN, p, Kd); mpz_sub(pIN, Kd, pIN);
  mpz_invert(qIN, q, Kd); mpz_sub(qIN, Kd, qIN);
  pIN_limb = *(pIN->_mp_d);
  qIN_limb = *(qIN->_mp_d);

  for (int i = 0; i < nQuery; ++i) {
    getmpzt(); mpz_init_set_str(c, buf, 10);
    calcm(); mpz_out_str(stdout, 10, m); putchar('\n');
  }
  return 0;
}
