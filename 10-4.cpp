#include<cstdio>
#include<cstdlib>
#include<string>
#include<map>
#include<iostream>
#include<cstring>
#define FOR(x,f,t) for(x=f;x<=t;++x)
using namespace std;
void getstr(unsigned n, char str[8]) {
    str[0] = 'a';
    str[1] = '0';
    str[2] = '0';
    str[3] = '0';
    str[4] = '0';
    str[5] = '0';
    str[6] = '0';
    str[7] = '0';
    int i = 2;
    while (n)
    {
        unsigned tmp = n % 36;
        if (tmp < 10)
            str[i++] = tmp + '0';
        else
        {
            str[i++] = tmp - 10 + 'a';
        }
        n = n / 36;
    }
}
typedef struct _hash_t{
    unsigned d[5];
    bool operator == (const _hash_t &oth){
        for(int i = 0; i < 5; i++)
            if(oth.d[i] != this->d[i]) return false;
        return true;
    }
} hash_t;
string R(hash_t sha, int i){
    char temp[9];
    getstr((sha.d[0] + sha.d[1] * i) % 2176782336, temp);
    temp[8] = '\0';
    return string(temp);
}
unsigned int SHA1_tmp;
#define SHA1_ROTL(a, b) (SHA1_tmp = (a), ((SHA1_tmp >> (32 - b)) & (0x7fffffff >> (31 - b))) | (SHA1_tmp << b))
#define SHA1_F(B, C, D, t) ((t < 40) ? ((t < 20) ? ((B & C) | ((~B) & D)) : (B ^ C ^ D)) : ((t < 60) ? ((B & C) | (B & D) | (C & D)) : (B ^ C ^ D)))
int UnitSHA1(const char *str, int length, unsigned sha1[5]) {
    unsigned char *pp, *ppend;
    unsigned int l, i, K[80], W[80], TEMP, A, B, C, D, E, H0, H1, H2, H3, H4;
    H0 = 0x67452301, H1 = 0xEFCDAB89, H2 = 0x98BADCFE, H3 = 0x10325476, H4 = 0xC3D2E1F0;
    for (i = 0; i < 20; K[i++] = 0x5A827999);
    for (i = 20; i < 40; K[i++] = 0x6ED9EBA1);
    for (i = 40; i < 60; K[i++] = 0x8F1BBCDC);
    for (i = 60; i < 80; K[i++] = 0xCA62C1D6);
    l = length + ((length % 64 > 56) ? (128 - length % 64) : (64 - length % 64));
    if (!(pp = (unsigned char *)malloc((unsigned int)l))) return -1;
    for (i = 0; i < length; pp[i + 3 - 2 * (i % 4)] = str[i], i++);
    for (pp[i + 3 - 2 * (i % 4)] = 128, i++; i < l; pp[i + 3 - 2 * (i % 4)] = 0, i++);
    *((unsigned int *)(pp + l - 4)) = length << 3;
    *((unsigned int *)(pp + l - 8)) = length >> 29;
    for (ppend = pp + l; pp < ppend; pp += 64)
    {
        for (i = 0; i < 16; W[i] = ((unsigned int *)pp)[i], i++);
        for (i = 16; i < 80; W[i] = SHA1_ROTL((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]), 1), i++);
        A = H0, B = H1, C = H2, D = H3, E = H4;
        for (i = 0; i < 80; i++)
        {
            TEMP = SHA1_ROTL(A, 5) + SHA1_F(B, C, D, i) + E + W[i] + K[i];
            E = D, D = C, C = SHA1_ROTL(B, 30), B = A, A = TEMP;
        }
        H0 += A, H1 += B, H2 += C, H3 += D, H4 += E;
    }
    free(pp - l);
    sha1[0] = H0, sha1[1] = H1, sha1[2] = H2, sha1[3] = H3, sha1[4] = H4;
    return 0;
}
hash_t SHA1(string val){
    char temp[8]; int i; hash_t ans;
    FOR(i,0,7) temp[i] = val[i];
    UnitSHA1(temp, 8, ans.d);
    return ans;
}
#define M 10005
string p0[M], pn[M];
int main(){
    int i,j,n;
    cin >> n;
    FOR(i,1,n) cin >> p0[i] >> pn[i];

    unsigned int sha_v[5];
    string sha;
    cin >> sha;
    
    FOR(i,0,4){
        unsigned temp = 0;
        FOR(j, i*8, (i+1)*8-1) {
            temp <<= 4;
            if ('0' <= sha[j] && sha[j] <= '9'){
                temp += sha[j] - '0';
            } else temp += sha[j] - 'a' + 10;
        }
        sha_v[i] = temp;
    }
    hash_t input_sha;
    FOR(i,0,4) input_sha.d[i] = sha_v[i];
    FOR(i,1,n){
        string val = p0[i];
        hash_t H = SHA1(val);
        for(int j = 0; j < 100000; j++){
            if(H == input_sha){
                cout << val << endl;
                exit(0);
            }
            val = R(H, j % 100 + 1);
            H = SHA1(val);
        }
    }
    printf("None\n");
    return 0;
}
