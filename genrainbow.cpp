#include<iostream>
#include <cstring>

using namespace std;

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
    str[0]='a';str[1]='0';str[2]='0';str[3]='0';str[4]='0';str[5]='0';str[6]='0';str[7]='0';
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

unsigned stepi(char str[8],int i)
{
    i=1+i%1000;
    unsigned sha1[5];
    StrSHA1(str,8,sha1);
    R(sha1,str,i);
    return str2uint(str);
}
FILE *fp=NULL;
unsigned long long *b;
#define is_in(i)  b[i>>6]&(1LL<<(i&0x3f))
#define set_int(i) b[i>>6]|=(1LL<<(i&0x3f))
unsigned int randstr(char str[8])
{
    unsigned int tmp=(rand()*rand()+rand())%2176782336;
    getstr(tmp,str);
    return tmp;
}

void genchain()
{
    unsigned int stri;
    int len=100000;
    char str[9];
    unsigned step,mycount;
    str[8]=0;
    b=(unsigned long long*)calloc(34012225,sizeof(long long));
    mycount=0;
    while(1)
    {
        if(mycount==2176782336)break;
        printf("count=%d\n",mycount);
        stri=randstr(str);
        if(is_in(stri))//已经存在于链表中
            continue;
        fprintf(fp,"%s ",str);set_int(stri);mycount++;
        for(int j=0;j<len;j++)
        {
            step=stepi(str,j);
            if(!(is_in(step)))
            {
                set_int(step);mycount++;
            }
        }
        fprintf(fp,"%s\n",str);
    }
}
void testSHA1()
{
    const char *pass="a0bcd7f3";
    unsigned sha1[5];
    StrSHA1(pass,strlen(pass),sha1);
    for(int i=0;i<5;i++)
        printf("%08X",sha1[i]);
    printf("\n");
    char str[9];
    str[8]=0;
    getstr(1119999,str);
    printf("%d\n",str2uint(str));
}
int main()
{
    testSHA1();
    fp=fopen("chaina0.txt","wt");
    genchain();
    fclose(fp);
    return 0;
}