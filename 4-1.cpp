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
#define KEYBYTES (16)
#define INPUTBYTES (1<<24)
#define SPNBYTES (8)
char key[KEYBYTES];
char x[SPNBYTES];
char y[SPNBYTES];
void work() {
    MEMCPY(y, x);
}
int main() {
    fread(key, KEYBYTES, 1, stdin);
    int i;
    FOR(i,1,(INPUTBYTES / SPNBYTES)) {
        fread(x, SPNBYTES, 1, stdin);
        work();
        fwrite(y, SPNBYTES, 1, stdout);
    }
	return 0;
}
