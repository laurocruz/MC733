#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int nesimo_primo(long int n, long int primos[]);
int ompPrime(long int n); 

int main() {
    long int n, ant = 1, tam = 1;
    long int* primos;
    scanf("%ld", &n);

    primos = (long int*) calloc(n, sizeof(long int));
    primos[0] = 2;

    if (n > 1) {
        for (int i = 2; i <= n; i++) {
            ant += 2;
            while (!nesimo_primo(ant, primos))
                ant += 2;
            primos[tam++] = ant;
        }
    } else if (n < 1) return 1;

    else ant++;

    printf("%d-ésimo primo: %d\n", n, ant);

    return 0;
}


