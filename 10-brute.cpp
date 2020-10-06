#include <iostream>
#include <stdio.h>
#include <string.h>
#define FOR(x,f,t) for(x=f;x<=t;++x)
using namespace std;
int StrSHA1(const char* str, long long length, unsigned sha1[5]);
void getstr(unsigned n,char str[8]);
unsigned str2uint(char str[8]);
void R(unsigned sha1[5],char str[8],int i);

char p0[10005][10],pn[10005][810];
char buf[45];
char p[10];
unsigned int q[5]; 
unsigned int vq[5]; 
char pp[10];
unsigned int qq[5]; 
int q_eq(unsigned int a[5], unsigned int b[5]) {
    int i;
    FOR(i,0,4) if(a[i]!=b[i]) return 0; //ne
    return 1; //eq
}
void debug_q(unsigned int q[5]) {
    int i;
    FOR(i,0,4) printf("%08x",q[i]);
    puts("");
}
void debug_p(char p[10]) {
    int i;
    FOR(i,0,7) putchar(p[i]);
    puts("");
}
int main() {
    int i,j,k,m;
    scanf("%d",&m);
    FOR(i,1,m) {
        scanf("%s",p0[i]);
        scanf("%s",pn[i]);
    }
    scanf("%s",buf);
    FOR(i,0,4) {
        int val=0;
        FOR(j,0,7) {
            char c=buf[i*8+j];
            val<<=4;
            val|=c>='a'?(c-'a'+10):(c-'0');
        }
        vq[i]=val;
    }
    // debug_q(q);
    FOR(j,1,m) {
        FOR(i,0,7) p[i]=p0[j][i];
        // debug_p(p);
        // debug_p(p);
        // debug_p(p);
        FOR(i,1,1) {
            // debug_p(p);
            StrSHA1(p,8,q); // q=sha1(p)
            debug_q(q);
            debug_q(q);
            debug_q(q);
            // debug_q(vq);
            if(q_eq(q,vq)) {
                printf("%s\n",p);
                return 0;
            }
            R(q,p,i%1000); // p=R(q)
        }
    }
    puts("None");
    return 0;
}
// source: genrainbow.cpp
long SHA1_tmp;
#define SHA1_ROTL(a,b) (SHA1_tmp=(a),((SHA1_tmp>>(32-b))&(0x7fffffff>>(31-b)))|(SHA1_tmp<<b))
#define SHA1_F(B,C,D,t) ((t<40)?((t<20)?((B&C)|((~B)&D)):(B^C^D)):((t<60)?((B&C)|(B&D)|(C&D)):(B^C^D)))
int StrSHA1(const char* str, long long length, unsigned sha1[5]){
    /*
    计算字符串SHA-1
    参数说明：
    str         字符串指针
    length      字符串长度
    sha1         用于保存SHA-1的字符串指针
    返回值为参数sha1
    */
    char *pp, *ppend;
    long l, i, K[80], W[80], TEMP, A, B, C, D, E, H0, H1, H2, H3, H4;
    H0 = 0x67452301, H1 = 0xEFCDAB89, H2 = 0x98BADCFE, H3 = 0x10325476, H4 = 0xC3D2E1F0;
    for (i = 0; i < 20; K[i++] = 0x5A827999);
    for (i = 20; i < 40; K[i++] = 0x6ED9EBA1);
    for (i = 40; i < 60; K[i++] = 0x8F1BBCDC);
    for (i = 60; i < 80; K[i++] = 0xCA62C1D6);
    l = length + ((length % 64 > 56) ? (128 - length % 64) : (64 - length % 64));
    if (!(pp = (char*)malloc((unsigned long)l))) return -1;
    for (i = 0; i < length; pp[i + 3 - 2 * (i % 4)] = str[i], i++);
    for (pp[i + 3 - 2 * (i % 4)] = 128,i++; i < l; pp[i + 3 - 2 * (i % 4)] = 0,i++);
    *((long*)(pp + l - 4)) = length << 3;
    *((long*)(pp + l - 8)) = length >> 29;
    for (ppend = pp + l; pp < ppend; pp += 64){
        for (i = 0; i < 16; W[i] = ((long*)pp)[i], i++);
        for (i = 16; i < 80; W[i] = SHA1_ROTL((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]), 1), i++);
        A = H0, B = H1, C = H2, D = H3, E = H4;
        for (i = 0; i < 80; i++){
            TEMP = SHA1_ROTL(A, 5) + SHA1_F(B, C, D, i) + E + W[i] + K[i];
            E = D, D = C, C = SHA1_ROTL(B, 30), B = A, A = TEMP;
        }
        H0 += A, H1 += B, H2 += C, H3 += D, H4 += E;
    }
    free(pp - l);
    sha1[0]=H0, sha1[1]=H1, sha1[2]=H2, sha1[3]=H3, sha1[4]=H4;
    return 0; 
}

void getstr(unsigned n,char str[8])
{
    str[0]='a';str[1]='0';str[2]='0';str[3]='0';
    str[4]='0';str[5]='0';str[6]='0';str[7]='0';
    int i=2;
    while(n)
    {
        unsigned tmp=n%36;
        if(tmp<10)
            str[i++]=tmp+'0';
        else
        {
            str[i++]=tmp-10+'a';
        }
        n=n/36;
    }
}
unsigned str2uint(char str[8])
{
    unsigned res=0;
    unsigned  tmp=0;
    unsigned  pn=1;
    for(int i=2;i<8;i++)
    {
        if(str[i]>='0' && str[i]<='9')
            tmp=str[i]-'0';
        else
        {
            tmp=str[i]-'a'+10;
        }
        res+=tmp*pn;
        pn*=36;
    }
    return res;
}
void R(unsigned sha1[5],char str[8],int i)
{
    getstr((sha1[0]+sha1[1]*i)%2176782336,str);
}
