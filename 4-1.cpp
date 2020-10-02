#include <stdio.h>
// #include <iostream> //?!
// #include <algorithm>
// #include <string>
// #include <cstring>
// #include <queue>
// #include <map>
// #include <vector>
// #include <cstdlib>
// #include <cmath>
using namespace std;
#define FOR(x,f,t) for(x=f;x<=t;++x)
#define RFOR(x,f,t) for(x=f;x>=t;--x) 
#define MEMSET(a,b) memset(a,b,sizeof(a))
#define MEMCPY(a,b) memcpy(a,b,sizeof(a))
#define oo 0x3f3f3f3f
#define ll long long
#define M 100005

#define KEYBYTES (16)
#define INPUTBYTES (1<<24)
#define SPNBYTES (8)

char key[KEYBYTES];
char x[SPNBYTES];
char y[SPNBYTES];
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
    static int sd=0x87755442;
	FOR(r,1,3) {
		int kr=(k>>(4*(5-r)))&0xFFFF;//16bit
		u=w^kr;
		int v=sbox16(u);
		w=pip(v);
        w^=sd;
        sd=(sd<<7)^(sd>>25);
	}
	int kr=(k>>4)&0xFFFF;
	u=w^kr; 
	int v=sbox16(u);
	kr=k&0xFFFF;
	int y=v^kr;
	return y;
} 
int main() {
    fread(key, KEYBYTES, 1, stdin);
    int i,j;
    int k1 = key[0] | (key[1]<<8) | (key[2]<<16) | (key[3]<<24);
    int k2 = key[4] | (key[5]<<8) | (key[6]<<16) | (key[7]<<24);
    int k[]={k1, k2};
    for(i=0;i<(INPUTBYTES / SPNBYTES); ++i) {
        fread(x, SPNBYTES, 1, stdin);
        FOR(j,0,(SPNBYTES/2)-1) {
            int in=x[2*j] | (x[2*j+1]<<8);
            int a = spn(in, k[i%2]);
            y[2*j]=a&0xFF;
            y[2*j+1]=(a>>8)&0xFF;
        }
        fwrite(y, SPNBYTES, 1, stdout);
    }
	return 0;
}
