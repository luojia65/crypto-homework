#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <queue>
#include <map>
#include <vector>
#include <cstdlib>
#include <cmath>
using namespace std;
#define FOR(x,f,t) for(x=f;x<=t;++x)
#define RFOR(x,f,t) for(x=f;x>=t;--x) 
#define MEMSET(a,b) memset(a,b,sizeof(a))
#define MEMCPY(a,b) memcpy(a,b,sizeof(a))
#define oo 0x3f3f3f3f
#define ll long long
#define M 100005
template<class T>void MAX(T &a,T b) {if(a<b)a=b;}
template<class T>void MIN(T &a,T b) {if(a>b)a=b;}
template<class T>void rd(T &a) { //只支持正整数或0
	char c;a=0;
	while(c=getchar(),!isdigit(c));
	do a=(a<<3)+(a<<1)+(c^48);
	while(c=getchar(),isdigit(c));
}	
struct bigint{
	int V[M],vl;
	bigint() {clear();}
	bigint(string a) {rd(a);}
	static const int L=4;
	static const int P=10000;
	void clear() {
		MEMSET(V,0);
		vl=0;
	}
	void rd(string s) {
		clear();
		int sl=s.length();
		int x,i,len=(sl-1)/L+1; //注意是sl-1，别打错
		FOR(i,0,len-1) {
			int end=sl-i*L;
			int start=max(0,end-L);
			sscanf(s.substr(start,end-start).c_str(), "%d", &x);
			V[vl++]=x;
		}
	}
	void rd(char *s,int sl) {
		clear();
		int x,i,len=(sl-1)/L+1; //注意是sl-1，别打错
		FOR(i,0,len-1) {
			int end=sl-i*L;
			int start=max(0,end-L);
			sscanf(s, "%d", &x);
			V[vl++]=x;
		}
	}
	void print() {
		int i,j;
		printf("%d",V[vl-1]);
		RFOR(i,vl-2,0) {
			char buf[20];
			sprintf(buf,"%04d",V[i]);
			FOR(j,0,strlen(buf)-1) putchar(buf[j]);
		}
	}
	bigint operator + (const bigint&b) const {
		bigint c;
		int i,g;
		for(i=0,g=0;;i++) {
			if(g==0&&i>=vl&&i>=b.vl) break;
			int x=g;
			if(i<vl) x+=V[i];
			if(i<b.vl) x+=b.V[i];
			c.V[c.vl++]=x%P;
			g=x/P;
		}
		return c;
	}
	bigint operator * (const bigint&b) const {
		bigint c("0");
		if((vl==1&&V[0]==0)||(b.vl==1&&b.V[0])) return c; //0*任何数=0
		c.vl=vl+b.vl;
		int i,j;
		FOR(i,0,vl-1) FOR(j,0,b.vl-1) {
			c.V[i+j]+=V[i]*b.V[j];
			if(c.V[i+j]>=P) {
				c.V[i+j+1]+=c.V[i+j]/P;
				c.V[i+j]%=P;
			}
		}
		while(c.vl>=2&&c.V[c.vl-1]==0) c.vl--;
		return c;
	}
	bigint operator / (const bigint &b) const {
		int i,j; bigint d("0"),a;MEMCPY(a.V,V),a.vl=vl;
		RFOR(i,vl-b.vl,0) {
			d.V[i]=0; int gt=1;
			while(gt) {
				FOR(j,0,b.vl-1) {
					a.V[i+j]-=b.V[j];
					if(a.V[i+j]<0) {
						a.V[i+j+1]-=a.V[i+j]/P;
						a.V[i+j]+=P;
					}
					d.V[i]+=1;
				}
				gt=0; 
				RFOR(j,b.vl-1,0) if(a.V[i+j]>b.V[j]) { gt=1;break; }
			}
		}
		d.vl=max(vl-b.vl,0);
		while(d.vl>=2&&d.V[d.vl-1]==0) d.vl--;
		return d;
	} 
	bool operator < (const bigint&b) {
		if(vl!=b.vl) return vl<b.vl;
		int i;
		RFOR(i,vl-1,0) if(V[i]!=b.V[i]) return V[i]<b.V[i];
		return false; //两数相等，不满足a<b，返回false 
	}
	bool operator == (const bigint&b) {
		if(vl!=b.vl) return false;
		int i;
		RFOR(i,vl-1,0) if(V[i]!=b.V[i]) return false;
		return true;
	}
};

bigint zero, one;

//bigint exgcd(bigint a, bigint b, bigint&x, bigint&y) {
//	if (b == zero)  { x=one, y=zero; return a; }
////	bigint ret=exgcd(b,a%b,y,x);
////	y=y-a/b*x;
////	return ret;
//}

//void solve() {
//	string a,b,c; cin>>a>>b>>c;
//	bigint e,p,q; e.rd(a), p.rd(b), q.rd(c);
//	bigint x,y;
//	bigint m=p*q;
//	bigint dd=exgcd(e,m,x,y);
//	bigint d=(x%m+m)%m;
//	d.print();
//	puts("");
//}

int main() {
//	zero.rd("0"); one.rd("1");
//	int N; rd(N);
//	int i; FOR(i,1,N) {
//		solve();
//	}
	while(1) {
		string a,b; cin>>a>>b; bigint c(a),d(b);
		c.print(); putchar('/');d.print();putchar('=');
		bigint e=c/d; e.print();
		printf("\n");
	}
	return 0;
}

