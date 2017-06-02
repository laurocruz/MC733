#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    long int a[1000000], b[1000000], c[1000000];
    int i, j, k;

    scanf("%d", &n);

    for(i = 0; i < n*n; i++)
        scanf("%ld", &a[i]);

    for(i = 0; i < n*n; i++)
        scanf("%ld", &b[i]);

    // MUL
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            c[i*n + j] = 0;
            for (k = 0; k < n; k++) {
                c[i*n + j] += a[i*n + k] * b[k*n + j];
            }
        }
    }
    return 0;
}
