#include <stdio.h>

int primo(int n) {
    int i;

    if (n % 2 == 0) {
        if (n != 2) return 0;
        else return 1;
    }


    for(i = 3; i < n; i += 2)
        if (n % i == 0)
            return 0;

    return 1;
}
