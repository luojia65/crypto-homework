#include <cstdio>
#include <cstdlib>
#include <ctime>

typedef unsigned int uint;
typedef unsigned long long ull;

const uint S[] = {
    14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7};
const uint fS[] = {
    14, 3, 4, 8, 1, 12, 10, 15, 7, 13, 9, 6, 11, 2, 0, 5};
const uint P[] = {
    0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};
const uint fP[] = {
    0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};
namespace SPN
{
    inline uint Sbox(uint text, const uint *sbox = S)
    {
#define sbox_unit(i) ret |= sbox[(text & (0xf << i)) >> i] << i;
        uint ret = 0;
        sbox_unit(0);
        sbox_unit(4);
        sbox_unit(8);
        sbox_unit(12);
        return ret;
    }

    inline uint Pbox(uint text, const uint *pbox = P)
    {
#define get_bit(x, i) ((x >> i) & 1)
#define pbox_unit(i) ret |= get_bit(text, i) << pbox[i];
#define pbox_unit_2(i) pbox_unit(i) pbox_unit(i + 1)
#define pbox_unit_4(i) pbox_unit_2(i) pbox_unit_2(i + 2)
#define pbox_unit_8(i) pbox_unit_4(i) pbox_unit_4(i + 4)
        uint ret = 0;
        pbox_unit_8(0);
        pbox_unit_8(8);
        return ret;
    }

#define Kbox(text, k, i) (text ^ ((k & (0xffff0000 >> (i * 4))) >> ((4 - i) * 4)))

    const uint SIZE_16 = (1 << 16);
    uint P_16[SIZE_16], S_16[SIZE_16];
    bool P_16_b[SIZE_16] = {0}, S_16_b[SIZE_16] = {0};

    void init()
    {
        for (int i = 0; i < SIZE_16; i++)
        {
            P_16[i] = Pbox(i);
            S_16[i] = Sbox(i);
        }
    }

    inline uint SPN_encryp(uint x, uint k)
    {
#define spn_enc_unit(i)      \
    x = Kbox(x, k, (i - 1)); \
    x = S_16[x];             \
    x = P_16[x];
#define spn_enc_unit_last(i) \
    x = Kbox(x, k, (i - 1)); \
    x = S_16[x];             \
    x = Kbox(x, k, i);

        spn_enc_unit(1);
        spn_enc_unit(2);
        spn_enc_unit(3);
        spn_enc_unit_last(4);
        return x;
    }
}; // namespace SPN

#define SIZE_16 (1 << 16)

uint crypto[SIZE_16];

char io_buf[SIZE_16 * 5 + 100];

#define read_hex_unit(i)                      \
    if ('0' <= io_buf[i] && io_buf[i] <= '9') \
        x = (x << 4) | (io_buf[i] - '0');     \
    else                                      \
        x = (x << 4) | (10 + io_buf[i] - 'a');

#define read_hex_unit_4(i) read_hex_unit(i) read_hex_unit(i + 1) read_hex_unit(i + 2) read_hex_unit(i + 3)

inline void read()
{
    fread(io_buf, sizeof(char), SIZE_16 * 5, stdin);
    for (int i = 0; i < SIZE_16; i++)
    {
        //scanf("%x", &crypto[i]);
        uint x = 0;
        read_hex_unit_4(i * 5);
        crypto[i] = x;
    }
}

const uint K = 0x3a093ca0;

const uint C_SIZE = (1 << 8);
int Count_1[C_SIZE] = {0};
#define lgb(x, i) ((x >> (16 - i)) & 1)
#define to_u4(x, k) fS[(x) ^ (k)];
#define unpack(v, group_len, group_index) ((v) >> (group_len * group_index))

int pair_cmp(const void *a, const void *b)
{
    uint ta = (*(uint *)a) & 0xffff;
    uint tb = (*(uint *)b) & 0xffff;
    if (ta < tb) {
        return 1;
    } else if (ta == tb) {
        return 0;
    } else {
        return -1;
    }
}

uint differ_analy(int n, int d = 0xb00)
{
    for (int i = 0; i < (1 << 8); i++)
        Count_1[i] = 0;
    for (int j = 0; j < n; j++)
    {
        uint x = j;
        uint dx = j ^ d;
        uint y = crypto[x];
        uint dy = crypto[dx];

        uint y1 = unpack(y, 4, 3) & 0xf;
        uint dy1 = unpack(dy, 4, 3) & 0xf;
        uint y3 = unpack(y, 4, 1) & 0xf;
        uint dy3 = unpack(dy, 4, 1) & 0xf;
        if (y1 != dy1 || y3 != dy3)
            continue;
        for (int i = 0; i < (1 << 8); i++)
        {
            uint L1 = (i & 0xf0) >> 4;
            uint L2 = i & 0x0f;

            uint u4_2 = to_u4(L1, unpack(y, 4, 2) & 0xf);
            uint u4_4 = to_u4(L2, y & 0xf);
            uint du4_2 = to_u4(L1, unpack(dy, 4, 2) & 0xf);
            uint du4_4 = to_u4(L2, dy & 0xf);

            uint ddu4_2 = u4_2 ^ du4_2;
            uint ddu4_4 = u4_4 ^ du4_4;
            if (ddu4_2 == 6 && ddu4_4 == 6)
            {
                Count_1[i]++;
            }
        }
    }
    int max = -1;
    uint max_key;
    for (int i = 0; i < (1 << 8); i++)
    {
        Count_1[i] |= (i << 24);
    }
    qsort(Count_1, (1 << 8), sizeof(uint), pair_cmp);
}

uint Count_2[C_SIZE];
uint differ_analy2(int n, int d = 0xb00)
{
    for (int i = 0; i < (1 << 8); i++)
        Count_2[i] = 0;
    for (int j = 0; j < n; j++)
    {
        uint x = j;
        uint dx = j ^ d;
        uint y = crypto[x];
        uint dy = crypto[dx];

        uint y1 = unpack(y, 4, 2) & 0xf;
        uint dy1 = unpack(dy, 4, 2) & 0xf;
        uint y3 = y & 0xf;
        uint dy3 = dy & 0xf;
        if (y1 != dy1 || y3 != dy3)
            continue;
        for (int i = 0; i < (1 << 8); i++)
        {
            uint L1 = (i & 0xf0) >> 4;
            uint L2 = i & 0x0f;

            uint u4_2 = to_u4(L1, unpack(y, 4, 3) & 0xf);
            uint u4_4 = to_u4(L2, (y >> 4) & 0xf);
            uint du4_2 = to_u4(L1, unpack(dy, 4, 3) & 0xf);
            uint du4_4 = to_u4(L2, (dy >> 4) & 0xf);

            uint ddu4_2 = u4_2 ^ du4_2;
            uint ddu4_4 = u4_4 ^ du4_4;
            if (ddu4_2 == 6 && ddu4_4 == 6)
            {
                Count_2[i]++;
            }
        }
    }
    int max = -1;
    uint max_key;
    for (int i = 0; i < (1 << 8); i++)
    {
        Count_2[i] |= (i << 24);
    }
    qsort(Count_2, (1 << 8), sizeof(uint), pair_cmp);
}

inline uint count_key(uint cover)
{
    for (int i = 0; i < (1 << 16); i++)
    {
        uint key = (i << 16) | cover;

        bool f = true;
        for (int j = 0; j < (1 << 16); j++)
        {
            uint x = j;
            uint y = crypto[j];
            if (SPN::SPN_encryp(x, key) != y)
            {
                f = false;
                break;
            }
        }
        if (f)
            return key;
    }
    return 0;
}


int main() {
    SPN::init();
    int ii,n;
    scanf("%d\n", &n);
    FOR(ii,1,n) {
        read();
        differ_analy(3000, 0xb00);
        differ_analy2(30000, 0xb00);
        uint key, flag = 0;
        for (int i = 0; i < 1; i++)
        {
            uint c1 = Count_1[i] & 0xff000000;
            c1 >>= 24;
            c1 = ((c1 & 0xf0) << 4) | (c1 & 0xf);
            for (int j = 0; j < 1; j++)
            {
                uint c2 = Count_2[j] & 0xff000000;
                c2 >>= 24;
                c2 = ((c2 & 0xf0) << 8) | ((c2 & 0x0f) << 4);
                key = count_key(c1 | c2);
                if (key != 0)
                {
                    flag = 1;
                    break;
                }
            }
            if (flag == 1)
                break;
        }
        //if(flag == 0) exit(1);
        printf("%08x\n", key);
    }
    return 0;
}