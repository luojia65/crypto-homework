#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
// #include <vector>
// #include <algorithm>
#define FOR(x,f,t) for(x=f;x<=t;++x)
#define RFOR(x,f,t) for(x=f;x>=t;--x)
#define MEMSET(a,b) memset(a,b,sizeof(a))

int p[8000][4] = { 0 }, c[8000][4];
int p_bin[8000][16] = { 0 };
int j = 0, i = 0;
int key[5][5] = { 0 };
int store1[20] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 };

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

int encrypt(int* p, int* K)
{
	int u[10], w[10], v[10];
	w[0] = p[0]; w[1] = p[1];
	w[2] = p[2]; w[3] = p[3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			u[j] = K[i + j] ^ w[j];
		}
		for (int i = 0; i < 4; i++)
		{
			v[i] = store1[u[i]];
		}
		w[0] = (((v[0] >> 3) & 0x1) << 3) + (((v[1] >> 3) & 0x1) << 2) + (((v[2] >> 3) & 0x1) << 1) + ((v[3] >> 3) & 0x1);
		w[1] = (((v[0] >> 2) & 0x1) << 3) + (((v[1] >> 2) & 0x1) << 2) + (((v[2] >> 2) & 0x1) << 1) + ((v[3] >> 2) & 0x1);
		w[2] = (((v[0] >> 1) & 0x1) << 3) + (((v[1] >> 1) & 0x1) << 2) + (((v[2] >> 1) & 0x1) << 1) + ((v[3] >> 1) & 0x1);
		w[3] = ((v[0] & 0x1) << 3) + ((v[1] & 0x1) << 2) + ((v[2] & 0x1) << 1) + (v[3] & 0x1);
	}
	for (int j = 0; j < 4; j++)
	{
		u[j] = K[3 + j] ^ w[j];
	}
	for (int i = 0; i < 4; i++)
	{
		v[i] = store1[u[i]];
	}
	for (int j = 0; j < 4; j++)
	{
		u[j] = K[4 + j] ^ v[j];
	}
	int ct = (u[0] << 12) + (u[1] << 8) + (u[2] << 4) + u[3];
	return ct;
}

int main()
{
	int n;
	scanf("%d", &n);
	getchar();
	int K[8] = { 0 };
	for (int k = 0; k < n; k++)
	{
		int store[20] = { 14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5 };
		int count[256] = { 0 }, count1[256] = { 0 };
		int u[4] = { 0 }, v[4] = { 0 }, u_bin[16] = { 0 };
		int max = -1, x = 0, y = 0, a = 0, b = 0;
		int z = 0;
		for (int i = 0; i < 8000; i++)
		{
			int num = 0;
			char ch = getchar();
			while (ch != ' ')
			{
				if (ch <= '9' && ch >= '0')
					p[i][num++] = ch - '0';
				else
					p[i][num++] = ch - 'a' + 10;
				ch = getchar();
			}
			ch = getchar(); num = 0;
			while (ch != '\n')
			{
				if (ch <= '9' && ch >= '0')
					c[i][num++] = ch - '0';
				else
					c[i][num++] = ch - 'a' + 10;
				ch = getchar();
			}
		}

		for (i = 0; i < 8000; i++)
		{
			int p_t[4] = { p[i][0] ,p[i][1],p[i][2],p[i][3] };
			for (int j = 0; j < 4; j++)
				for (int m = 3; m >= 0; m--)
				{
					p_bin[i][4 * j + m] = p_t[j] & 0x1;
					p_t[j] >>= 1;
				}
			int bin = p_bin[i][4] ^ p_bin[i][6] ^ p_bin[i][7];
			//解密
			for (K[5] = 0; K[5] < 16; K[5]++)
			{
				v[1] = K[5] ^ c[i][1];
				u[1] = store[v[1]];
				u_bin[5] = (u[1] & 0x4) >> 2;
				u_bin[7] = (u[1] & 0x1);
				for (K[7] = 0; K[7] < 16; K[7]++)
				{
					v[3] = K[7] ^ c[i][3];
					u[3] = store[v[3]];
					u_bin[13] = (u[3] & 0x4) >> 2;
					u_bin[15] = u[3] & 0x1;
					if ((bin ^ u_bin[5] ^ u_bin[7] ^ u_bin[13] ^ u_bin[15]) == 0)
						count[(K[5] << 4) + K[7]]++;
				}
			}
		}
		//计数
		int flag = 0;
		for (int i = 0; i < 256; i++)
		{
			count[i] = abs(count[i] - 4000);
		}
		while (1)
		{
			int count0[256] = { 0 };
			max = -1;
			for (int m = 0; m < 256; m++)
			{
				if (count[m] > max)
				{
					max = count[m];
					x = (m & 0x00f0) >> 4; y = m & 0x0f;
				}
			}
			K[5] = x;
			K[7] = y;
			count[(K[5] << 4) + K[7]] = 0;
			for (int o = 0; o < 8000; o++)
			{
				int bin = p_bin[o][4] ^ p_bin[o][5];
				v[1] = K[5] ^ c[o][1];
				u[1] = store[v[1]];
				u_bin[7] = u[1] & 0x1;
				u_bin[5] = (u[1] & 0x4) >> 2;
				for (K[4] = 0; K[4] < 16; K[4]++)
				{
					v[0] = K[4] ^ c[o][0];
					u[0] = store[v[0]];
					u_bin[3] = u[0] & 0x1;
					u_bin[1] = (u[0] & 0x4) >> 2;
					for (K[6] = 0; K[6] < 16; K[6]++)
					{
						v[2] = K[6] ^ c[o][2];
						u[2] = store[v[2]];
						u_bin[11] = u[2] & 0x1;
						u_bin[9] = (u[2] & 0x4) >> 2;
						if ((bin ^ u_bin[1] ^ u_bin[3] ^ u_bin[5] ^ u_bin[7] ^ u_bin[9] ^ u_bin[11]) == 0)
						{
							count0[(K[4] << 4) + K[6]]++;
						}
					}
				}
			}
			max = -1;
			for (int i = 0; i < 256; i++)
			{
				count0[i] = abs(count0[i] - 4000);
			}
			for (int m = 0; m < 256; m++)
			{
				if (count0[m] > max)
				{
					max = count0[m];
					a = (m & 0xf0) >> 4; b = m & 0x0f;
				}
			}
			K[4] = a;
			K[6] = b;
			int c_f = (c[0][0] << 12) + (c[0][1] << 8) + (c[0][2] << 4) + c[0][3];
			int c_f1 = (c[1][0] << 12) + (c[1][1] << 8) + (c[1][2] << 4) + c[1][3];
			//解密
			for (K[0] = 0; K[0] < 16; K[0]++)
			{
				for (K[1] = 0; K[1] < 16; K[1]++)
				{
					for (K[2] = 0; K[2] < 16; K[2]++)
					{
						for (K[3] = 0; K[3] < 16; K[3]++)
						{
							if(flag = (encrypt(p[0], K) == c_f) && (encrypt(p[1], K) == c_f1))
								break;
						}
						if (flag)
							break;
					}
					if (flag)
						break;
				}
				if (flag)
					break;
			}
			if (flag)
				break;
		}
		char buf[9] = { 0 };
		for (int i = 0; i < 8; i++)
		{
			if (K[i] < 10) buf[i] = K[i] + '0';
			else buf[i] = K[i] + 'a' - 10;
		}
		fwrite(buf, 1, 8, stdout);
		putchar('\n');
	}
	return 0;
}
