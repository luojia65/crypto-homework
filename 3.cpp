#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#define FOR(x,f,t) for(x=f;x<=t;++x)
#define RFOR(x,f,t) for(x=f;x>=t;--x)

inline int rd16() {
    int x=0;char ch=getchar();
    while((ch<'0'||ch>'9')&&(ch<'a'||ch>'f')){
        ch=getchar();
    }
    while((ch>='0'&&ch<='9')||(ch>='a'&&ch<='f')){
        if(ch>='0'&&ch<='9')x=x*16+ch-'0';
        else if(ch>='a'&&ch<='f')x=x*16+ch-'a'+10;
        ch=getchar();
    }
    return x;
}

inline void print16(int a) {
	int i;
	FOR(i,1,4) {
		int c=(a>>(4*(4-i)))&0xF;
		if(c>=0&&c<=9) putchar('0'+c);
		else if (c>=10&&c<=15) putchar('a'+c-10);
	}
}

#define AAA 439
#define BBB 396
#define CCC 777

#define CNT2MAX 16

int n;
int pi_s[] = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7}; 
int pi_p[] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15}; // symmetry !
int pow2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};
// spn args
int x[250], y[250], x1[250], y1_[250];
int u1, u2, u3, u4;
unsigned l;
int maxkey24, maxkey13, max;
unsigned key0, key;
unsigned raw_key;
int k1, k2, k3, k4, k5;
int cnt[65536];
int cns[65536];
int s[65536], p[65536], r[65536], q[65536];
int cipher[65536];

inline void preprocess24() {
  int k=0;
  while (k < 80) {
  	x[k] = rand()%65536;
  	x1[k] = x[k]^0x0b00;
  	y[k] = cipher[x[k]];
  	y1_[k] = cipher[x1[k]];
  	if(!((y[k]^y1_[k])&0xf0f0)) ++k;
  }
}
inline void preprocess134() {
  int k=0;
  while (k < 240) {
  	x[k] = rand()%65536;
  	x1[k] = x[k]^0x0f00;
  	y[k] = cipher[x[k]];
  	y1_[k] = cipher[x1[k]];
  	if(!((y[k]^y1_[k])&0x0f00)) ++k;
  }
}

int main() {
    int i,j,k,n;
    FOR(i,0,65535) {
        s[i] = (pi_s[i>>12]<<12) | (pi_s[(i>>8)&0xf]<<8) | (pi_s[(i>>4)&0xf]<<4) | (pi_s[i&0xf]);
        r[s[i]] = i;
        p[i] = 0;
        for (int j = 0; j < 16; ++j) { // FIXME
        if (pow2[j] & i) p[i] |= pow2[pi_p[j]];
        }
        q[p[i]] = i;
    }
    scanf("%d",&n);
    FOR(k,0,n-1) {
        for (i = 0; i < 65536; ++i) cipher[i] = rd16();
        int find = 0;

        // first 8 bit
        preprocess24();
        memset(cnt, 0, 256 * sizeof(int));
        for (i = 0; i < 80; ++i) {
            for (l = 0; l <= 0xff; ++l) {
                u2 = r[(l>>4)^((y[i]&0x0f00)>>8)] ^ r[(l>>4)^((y1_[i]&0x0f00)>>8)];
                u4 = r[(l&0x0f)^(y[i]&0x000f)] ^ r[(l&0x0f)^(y1_[i]&0x000f)];
                if (!(u2^6) && !(u4^6)) ++cnt[l];
            }
        }

        // second 8 bit
        for (; ;){
            int max = -1;
            for(l = 0; l <= 0xff; ++l) {
                if(cnt[l] > max) {
                    max = cnt[l];
                    maxkey24 = l;
                }
            }
        cnt[maxkey24] = 0;
        preprocess134();
        memset(cns, 0, 256 * sizeof(int));
        for (i = 0; i < 240; ++i) {
            for (l = 0; l <= 0xff; ++l) {
                u1 = r[(l>>4)^((y[i]&0xf000)>>12)] ^ r[(l>>4)^((y1_[i]&0xf000)>>12)];
                u3 = r[(l&0x0f)^((y[i]&0x00f0)>>4)] ^ r[(l&0x0f)^((y1_[i]&0x00f0)>>4)];
                u4 = r[(maxkey24&0x0f)^(y[i]&0x000f)] ^ r[(maxkey24&0x0f)^(y1_[i]&0x000f)];
                if (!(u1^6) & !(u3^6) && !(u4^6)) ++cns[l];
            }
        }

        // other 16 bit
        for (int cnt2 = 0; cnt2 < CNT2MAX; ++cnt2) {
            max = -1;
            for(l = 0; l <= 0xff; ++l) {
                if(cns[l] > max) {
                    max = cns[l];
                    maxkey13 = l;
                }
            }
            cns[maxkey13] = 0;
            for (key0 = 0; key0 <= 0xffff; ++key0) {
                key = (key0<<16) | ((maxkey13&0xf0)<<8) | ((maxkey24<<4)&0x0f00) | ((maxkey13&0xf)<<4) | (maxkey24&0xf);
                k5 = key & 0xffff;
                k4 = (key>>4) & 0xffff;
                k3 = (key>>8) & 0xffff;
                k2 = (key>>12) & 0xffff;
                k1 = (key>>16) & 0xffff;
                if((s[p[s[p[s[p[s[AAA^k1]]^k2]]^k3]]^k4] ^ k5) == cipher[AAA])
                if((s[p[s[p[s[p[s[BBB^k1]]^k2]]^k3]]^k4] ^ k5) == cipher[BBB])
                    if((s[p[s[p[s[p[s[CCC^k1]]^k2]]^k3]]^k4] ^ k5) == cipher[CCC])
                    {find = 1; break;}
                }
                if (find) break;
            }
            if (find) break;
        }
        // put key
        int buf[8];
        for (int i = 7; i >= 0; --i) {
            buf[i] = key & 0xf;
            if (buf[i] < 10) buf[i] += '0';
            else buf[i] += 'a' - 10;
            key >>= 4;
        }
        fwrite(buf, 1, 8, stdout);
        putchar('\n');
    }

    return 0;
}
