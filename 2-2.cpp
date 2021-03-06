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

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
// #include <vector>
// #include <algorithm>
#define FOR(x,f,t) for(x=f;x<=t;++x)
#define RFOR(x,f,t) for(x=f;x>=t;--x)
#define MEMSET(a,b) memset(a,b,sizeof(a))
#define M 8005
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
inline int abs(int a) {
	if(a>0) return a; else return -a;
}
int X[10005], Y[10005]; // idx in 1..=N
int cnt[20][20]; 
int cnt2[20][20]; 
/*
acd9 

1              16
1321312011010010

cc3d

1                16
1321,2312,3123,1231

*/
struct Z{int l,m,c;};
bool my_cmp(const Z&a, const Z&b) {
	return a.c>b.c;
}
#define T 8000
#define x(i) ((X[i]>>(16-i))&0x1)

int main() {
	// freopen("2.in","r",stdin); 
	int ii,i,j,l,m,N; scanf("%d",&N);
	FOR(ii,1,N) {
		FOR(i,1,8000) X[i]=rd16(),Y[i]=rd16(); // ����,���� 
		MEMSET(cnt,0);
		FOR(i,1,8000){
//			printf("i=%d,X[i]=%x,Y[i]=%x\n",i,X[i],Y[i]);
			int y2=(Y[i]>>8)&0xF,y4=Y[i]&0xF;
			int x5=(X[i]>>11)&0x1,x7=(X[i]>>9)&0x1;
			int x8=(X[i]>>8)&0x1;
			int xx=x5^x7^x8;
//			printf("y2=%x,y4=%x,",y2,y4);
			FOR(l,0,15) {
				int v42=l^y2;
				int u42=pis2(v42);
				int u4b6=(u42>>2)&0x1,u4b8=u42&0x1;
				int yy=xx^u4b6^u4b8;
				FOR(m,0,15) {
					int v44=m^y4;
	//				printf("v42=%x,v44=%x,",v42,v44);
					int u44=pis2(v44);
	//				printf("u42=%x,u44=%x,",u42,u44);
					int u4b14=(u44>>2)&0x1,u4b16=u44&0x1;
					int z=yy^u4b14^u4b16;
	//				printf("z=%d\n",z);
					if (z==0) {
						cnt[l][m]+=1;
					}
				}
			}
		}
// 		int mx=-1,kkl1=0,kkm1=0; //kkl1->2 kkm1->4 
		FOR(l,0,15) FOR(m,0,15) {
			cnt[l][m]=abs(cnt[l][m]-T/2);
// //				printf("l=%d, m=%d, cnt=%d\n",l,m,cnt[l][m]); 
// 			if(cnt[l][m]>mx) {
// 				mx=cnt[l][m];
// 				kkl1=l,kkm1=m;
// 			}
		}
		int g;
		FOR(g,0,20) {
			int kkl1,kkm1;
			int mx1=-1;
			FOR(l,0,15) FOR(m,0,15) {
				if(cnt[l][m]>mx1) {
					mx1=cnt[l][m];
					kkl1=l,kkm1=m;
				}
			}
			// printf("kkl1=%d kkm1=%d\n",kkl1,kkm1);
			// MEMCPY(cnt2,cnt);
			MEMSET(cnt2, 0);
			// cnt[l][m]=0;
			FOR(i,1,8000){
				int y1=(Y[i]>>12)&0xF,y2=(Y[i]>>8)&0xF,y3=(Y[i]>>4)&0xF;
				// int y4=Y[i]&0xF;
				int u42=pis2(kkl1^y2);
				int u4b6=(u42>>2)&0x1,u4b8=u42&0x1;
				int x5=(X[i]>>(16-5))&0x1;
				int x6=(X[i]>>(16-6))&0x1;
				int xx=x5^x6^u4b6^u4b8;
				FOR(l,0,15) {
					int v41=l^y1;
					int u41=pis2(v41);
					int u4b2=((u41>>2)&0x1),u4b4=u41&0x1;
					int yy=xx^u4b2^u4b4;
					FOR(m,0,15) {
						int v43=m^y3;
						int u43=pis2(v43);
						int u4b10=(u43>>2)&0x1,u4b12=u43&0x1;
						int z=/*5,6*/
						/* 2,4,6,8,10,12*/
							yy^u4b10^u4b12;
						// if(i==10)printf("l=%d, m=%d, d=%d\n",l,m,z);
						if (z==0) {
							// printf("%d %d %d\n",i,l,m);
							cnt2[l][m]+=1;
						}
					}
				}
			}
			int mx=-1;int kkl2=0,kkm2=0;
			FOR(l,0,15) FOR(m,0,15) {
				cnt2[l][m]=abs(cnt2[l][m]-T/2);
					// printf("l=%d, m=%d, cnt=%d\n",l,m,cnt[l][m]); 
				if(cnt2[l][m]>mx) {
					mx=cnt2[l][m];
					kkl2=l,kkm2=m;
				}
			}
			// printf("%d %d\n",kkl2,kkm2);
			
			// [i[20][4..=23]][kkl[4]: 20..=23]
			// .. [i[0..=3][4]][kkm[4]: 28..=31] 
			// int x,y,z,a; // 32bits - 4*4bits = 16bits
			int kt= (kkl2<<12) | (kkl1<<8) | (kkm2<<4) | kkm1;
			FOR(i,0,((2<<16)-1)) {
				int k=(i<<16) | kt;
				// printf("%x\n",k);
				if(spn(X[1],k)==Y[1]) {
					if(spn(X[2],k)==Y[2]) {
						print32(k);
						puts("");
						goto nxt_loop_out;
					}
				}
			}
			cnt[kkl1][kkm1] =0; // failed
		}
		nxt_loop_out:;
		// ���һ�У�8��16�����ַ�����ʾ32���ص���Կ 
	}
	return 0;
}
