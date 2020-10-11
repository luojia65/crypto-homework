#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-fwhole-program")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-fstrict-overflow")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-skip-blocks")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("-funsafe-loop-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")
#pragma GCC optimize(-fsanitize=address)

#include <cstdio>
#include <cstdlib>
#include <cstring>
#define FOR(x,f,t) for(x=f;x<=t;++x)
#define RFOR(x,f,t) for(x=f;x>=t;--x)
#define MEMSET(a,b) memset(a,b,sizeof(a))

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
	// int i;//x=0;char ch=getchar();
	// char buf[5];
	// FOR(i,0,3) {
	// 	buf[i] = getchar();
	// 	if (buf[i] >= '0' && buf[i] <= '9') {
	// 		buf[i] -= '0';
	// 		i++;
	// 	} else { // if (buf[i] >= 'a' && buf[i] <= 'f') {
	// 		buf[i] += 10 - 'a';
	// 		i++;
	// 	}
	// }
	// getchar();
	// return (buf[0] << 12) | (buf[1] << 8) | (buf[2] << 4) | buf[3];
}

inline void print16(int a) {
	int i;
	FOR(i,1,4) {
		int c=(a>>(4*(4-i)))&0xF;
		if(c>=0&&c<=9) putchar('0'+c);
		else if (c>=10&&c<=15) putchar('a'+c-10);
	}
}
inline void print32(int a) {
	int i;
	FOR(i,1,8) {
		int c=(a>>(4*(8-i)))&0xF;
		if(c>=0&&c<=9) putchar('0'+c);
		else if (c>=10&&c<=15) putchar('a'+c-10);
	}
}
int SBOX[]={
	0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8,
	0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7
};
int PBOX[]={ 
	1,5,9,13,	2,6,10,14,
	3,7,11,15,	4,8,12,16
};
// E4D1_2FB8_3A6C_5907
inline int pis(int y) {
	int A[]={
		0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8,
		0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7
	};
	return A[y];
}
inline int sbox16(int u) {
	int v=0,j;
	FOR(j,1,4) {
		int vri = (u>>(4*(j-1)))&0xF;
		v|=SBOX[vri]<<(4*(j-1));
	}
	return v;
}
// E348_1CAF_7D96_B205
inline int pis2(int y) {
	int A[]={
		0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF,
		0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5
	};
	return A[y];
} 
inline int pip(int y) {
	int idx[]={0, 
		1,5,9,13,	2,6,10,14,
		3,7,11,15,	4,8,12,16
	},i,ans=0;
	FOR(i,1,16) {
		int bit=(y>>(i-1))&0x1;
		ans|=bit<<(idx[i]-1);
	}
	return ans;
}
inline int spn(int x,int k) {
	int r,w=x,u,j;
	FOR(r,1,3) {
		int kr=(k>>(4*(5-r)))&0xFFFF;//16bit
		u=w^kr;
		int v=sbox16(u);
		w=pip(v);
	}
	int kr=(k>>4)&0xFFFF;
	u=w^kr; 
	int v=sbox16(u);
	kr=k&0xFFFF;
	int y=v^kr;
	return y;
} 

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
int V[65536];

#define AAA 500
#define BBB 600
#define CCC 700

int main() {
	int i,j,k,ii,n;
	scanf("%d",&n);
	// getchar();
	FOR(i,0,65535) {
		s[i] = (pi_s[i>>12]<<12) | (pi_s[(i>>8)&0xf]<<8) | (pi_s[(i>>4)&0xf]<<4) | (pi_s[i&0xf]);
		r[s[i]] = i;
		p[i] = 0;
		for (int j = 0; j < 16; ++j) {
			if (pow2[j] & i) p[i] |= pow2[pi_p[j]];
		}
		q[p[i]] = i;
	}
	FOR(ii,1,n) {
		FOR(i,0,65535) V[i] = rd16();
		int find = 0;

		// get first 8 bit
		int k=1;
		while (k <= 80) {
			x[k] = rand()%65536;
			x1[k] = x[k]^0x0b00;
			y[k] = V[x[k]];
			y1_[k] = V[x1[k]];
			if(!((y[k]^y1_[k])&0xf0f0)) ++k;
		}
		MEMSET(cnt,0);
		FOR(i,1,80) {
			for (l = 0; l <= 0xff; ++l) {
				u2 = r[(l>>4)^((y[i]&0x0f00)>>8)] ^ r[(l>>4)^((y1_[i]&0x0f00)>>8)];
				u4 = r[(l&0x0f)^(y[i]&0x000f)] ^ r[(l&0x0f)^(y1_[i]&0x000f)];
				if (!(u2^6) && !(u4^6)) ++cnt[l];
			}
		}

		// second 8 bit
		while(1){
			int max = -1;
			FOR(l,0,255) {
				if(cnt[l] > max) {
					max = cnt[l];
					maxkey24 = l;
				}
			}
			cnt[maxkey24] = 0;
			int k=1;
			while (k <= 240) {
				x[k] = rand()%65536;
				x1[k] = x[k]^0x0f00;
				y[k] = V[x[k]];
				y1_[k] = V[x1[k]];
				if(!((y[k]^y1_[k])&0x0f00)) ++k;
			}
			MEMSET(cns, 0);
			FOR(i,1,240) {
				FOR(l,0,255) {
					u1 = r[(l>>4)^((y[i]&0xf000)>>12)] ^ r[(l>>4)^((y1_[i]&0xf000)>>12)];
					u3 = r[(l&0x0f)^((y[i]&0x00f0)>>4)] ^ r[(l&0x0f)^((y1_[i]&0x00f0)>>4)];
					u4 = r[(maxkey24&0x0f)^(y[i]&0x000f)] ^ r[(maxkey24&0x0f)^(y1_[i]&0x000f)];
					if (!(u1^6) & !(u3^6) && !(u4^6)) ++cns[l];
				}
			}

			// other 16 bit
			int cnt2;
			FOR(cnt2,1,16) {
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
					if((s[p[s[p[s[p[s[AAA^k1]]^k2]]^k3]]^k4] ^ k5) == V[AAA])
					if((s[p[s[p[s[p[s[BBB^k1]]^k2]]^k3]]^k4] ^ k5) == V[BBB])
						if((s[p[s[p[s[p[s[CCC^k1]]^k2]]^k3]]^k4] ^ k5) == V[CCC])
						{find = 1; break;}
				}
				if (find) break;
			}
			if (find) break;
		}
		print32(key);
		putchar('\n');
	}

	return 0;
}
