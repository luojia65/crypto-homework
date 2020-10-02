#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <vector>
#include <algorithm>
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
//			printf("y2=%x,y4=%x,",y2,y4);
			FOR(l,0,15) {
				int v42=l^y2;
				int u42=pis2(v42);
				int u4b6=(u42>>2)&0x1,u4b8=u42&0x1;
				FOR(m,0,15) {
					int v44=m^y4;
	//				printf("v42=%x,v44=%x,",v42,v44);
					int u44=pis2(v44);
	//				printf("u42=%x,u44=%x,",u42,u44);
					int u4b14=(u44>>2)&0x1,u4b16=u44&0x1;
					int z=x5^x7^x8^u4b6^u4b8^u4b14^u4b16;
	//				printf("z=%d\n",z);
					if (z==0) {
						cnt[l][m]+=1;
					}
				}
			}
		}
		int mx=-1,kkl1=0,kkm1=0; //kkl1->2 kkm1->4 
		FOR(l,0,15) FOR(m,0,15) {
			cnt[l][m]=abs(cnt[l][m]-T/2);
//				printf("l=%d, m=%d, cnt=%d\n",l,m,cnt[l][m]); 
			if(cnt[l][m]>mx) {
				mx=cnt[l][m];
				kkl1=l,kkm1=m;
			}
		}
		// printf("kkl1=%d kkm1=%d\n",kkl1,kkm1);
		// MEMCPY(cnt2,cnt);
		MEMSET(cnt, 0);
		// cnt[l][m]=0;
		FOR(i,1,8000){
			int y1=(Y[i]>>12)&0xF;
			int y2=(Y[i]>>8)&0xF;
			int y3=(Y[i]>>4)&0xF;
			// int y4=Y[i]&0xF;
			FOR(l,0,15) FOR(m,0,15) {
				int v41=l^y1,v43=m^y3;
				int u41=pis2(v41),u43=pis2(v43);
				int u42=pis2(kkl1^y2);
				// int u44=pis2(kkm1^y4);
				int x5=(X[i]>>(16-5))&0x1;
				int x6=(X[i]>>(16-6))&0x1;
				int z=x5^x6^ /*5,6*/
				/* 2,4,6,8,10,12*/
					((u41>>2)&0x1)^ (u41&0x1)^
					((u42>>2)&0x1)^ (u42&0x1)^
					((u43>>2)&0x1)^ (u43&0x1) ^ 1;
					
                        // if(i==10)printf("l=%d, m=%d, d=%d\n",l,m,z);
				if (z==0) {
                    // printf("%d %d %d\n",i,l,m);
					cnt[l][m]+=1;
				}
			}
		}
		FOR(l,0,15) FOR(m,0,15) {
			cnt[l][m]=abs(cnt[l][m]-T/2);
		}
		// std::vector<Z> vz;
		// FOR(l,0,15) FOR(m,0,15) 
		// 	vz.push_back((Z) { l, m, cnt[l][m] });
		// std::sort(vz.begin(), vz.end(), my_cmp);
		// FOR(i,0,100) printf("l=%d, m=%d, c=%d\n",vz[i].l,vz[i].m,vz[i].c);
		
		mx=-1;int kkl2=0,kkm2=0;
		FOR(l,0,15) FOR(m,0,15) {
				// printf("l=%d, m=%d, cnt=%d\n",l,m,cnt[l][m]); 
			if(cnt[l][m]>mx) {
				mx=cnt[l][m];
				kkl2=l,kkm2=m;
			}
		}
		// printf("%d %d\n",kkl2,kkm2);
		
		// [i[20][4..=23]][kkl[4]: 20..=23]
		// .. [i[0..=3][4]][kkm[4]: 28..=31] 
		FOR(i,0,(2<<16)-1) { // 32bits - 2*4bits = 28bits
			int k=(i<<16) | (kkl2<<12) | (kkl1<<8) | (kkm2<<4) | kkm1;
			// printf("%x\n",k);
			int ff=1;
			FOR(j,1,4) {
				int yy=spn(X[j],k);
				// printf("%x\n",yy);
				if(yy!=Y[j]) {
					ff=0;
					continue;
				}
			}
			if(ff) {
				printf("%x\n",k);
				break;
			}
		} 
		// ���һ�У�8��16�����ַ�����ʾ32���ص���Կ 
	}
	return 0;
}

