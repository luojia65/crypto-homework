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
		v|=pis(vri)<<(4*(j-1));
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
inline int sbox162(int u) {
	int v=0,j;
	FOR(j,1,4) {
		int vri = (u>>(4*(j-1)))&0xF;
		v|=pis2(vri)<<(4*(j-1));
	}
	return v;
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

int spn(int x,int k) {
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
//int debug(const char*ss,int a) {
//    char s[20];
//	itoa(a, s, 2);
//	printf(ss);
//	printf("%016s\n", s);
//}

int main() {
    int i,N=8000,j,n=10;
    printf("%d\n",n);
    FOR(j,1,n) {
        int k=((rand()&0xFFFF) << 16) | (rand() & 0xFFFF);
        fprintf(stderr, "key = %08x\n", k);
        FOR(i,1,N) {
            int a=rand() % 0xFFFF;
            int b=spn(a,k);
            printf("%04x %04x\n",a,b);
        }
    }
    return 0;
}
