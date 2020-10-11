#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
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
}

inline void print16(int a) {
	int i;
	FOR(i,1,4) {
		int c=(a>>(4*(4-i)))&0xF;
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

/*
第一行输入n，接下来n行每行4*65536个字符，表示0x0000至0xffff对应的密文，每个密文之间空格隔开。
前三组n=1，第四组n=10，第五组n=50，第六组n=100， 第七组n=500

输出n行，每行为对应的密钥。
*/
int V[100005];
int cnt[20][20];
#define MAX 100
int x[MAX],x8[MAX],y[MAX],y8[MAX];
int main() {
    int i,j,k,l,n;
    scanf("%d",&n);
    FOR(i,1,n) {
        // 读入！
        FOR(j,0,65535) V[j]=rd16();
        MEMSET(cnt,0);
        k=1;
        while (k <= 80) {
            x[k] = rand() % 65536;
            x8[k] = x[k] ^ 0x0b00;
            y[k] = V[x[k]];
            y8[k] = V[x8[k]];
            if(((y[k]^y8[k])&0xf0f0) != 0) k+=1;
        }
		FOR(j,1,80) {
            // if y&0xF==y8&0xF && y&0xF00==y8&0xF00 
            FOR(k,0,15) {
                FOR(l,0,15) {
                    int v42=k^((y[j]>>1)&0xF);
                    int v44=k^((y[j]>>3)&0xF);
                    int u42=pis2(v42);
                    int u44=pis2(v44);
                    int v42_8=k^((y8[j]>>1)&0xF);
                    int v44_8=k^((y8[j]>>3)&0xF);
                    int u42_8=pis2(v42_8);
                    int u44_8=pis2(v44_8);
                    int u42_p=u42^u42_8;
                    int u44_p=u44^u44_8;
                    if (u42_p==0b0110 && u44_p==0b0110) {
                        cnt[k][l]+=1;
                    }
                }
            }
		}
        int mx=1; int mxk,mxl;
        FOR(k,0,15) {
            FOR(l,0,15) {
                if (cnt[k][l]>mx) {
                    mx=cnt[k][l];
                    mxk=k, mxl=l;
                }
            }
        }
        printf("%x %x\n",mxk,mxl);
    }
    return 0;
}