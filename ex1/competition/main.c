#include <stdio.h>
#include <stdlib.h>

int nesimo_primo(long int n, long int primos[], long int* tam);

int main() {
    long int n, ant = 1, tam = 1;
    long int* primos;
    scanf("%ld", &n);

    primos = (long int*) calloc(n, sizeof(long int));
    primos[0] = 2;

    if (n > 1) {
        for (int i = 2; i <= n; i++) {
            ant += 2;
            while (!nesimo_primo(ant, primos, &tam))
                ant += 2;
        }
    } else if (n < 1) return 1;

    printf("%d-ésimo primo: %d\n", n, ant);

    return 0;
}


int nesimo_primo(long int n, long int primos[], long int* tam) {
    int i;

    for(i = 0; primos[i]*primos[i] <= n; i++)
        if (n % primos[i] == 0)
            return 0;

    primos[(*tam)++] = n;
    return 1;

}
