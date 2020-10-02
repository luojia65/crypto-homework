#include <stdio.h>
#include <stdlib.h>
#define FOR(x,f,t) for(x=f;x<=t;++x)
int main() {
    freopen("5-data.in","w",stdout);
    int i,N; N=1000; printf("%d\n",N);
    FOR(i,1,N) {
        int j;
        FOR(j,1,3) {
            int k;
            FOR(k,1,1) {
                printf("%d",rand());
            }
            printf(" ");
        }
        printf("\n");
    }
    return 0;
}
