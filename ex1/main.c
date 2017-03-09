#include <stdio.h>

int primo(int n);
/*
int primo(int n) {
    int i;

    for(i = 2; i < n; i ++)
        if (n % i == 0)
            return 0;

    return 1;
}
*/

int main() {
    int n, x = 0;
    scanf("%d", &n);

    if (n >= 2) x++;

    for (int i = 3; i <= n; i += 2) {
        if (primo(i)) x++;
    }

    printf("Primos entre 1 e %d: %d\n", n, x);

    return 0;
}
