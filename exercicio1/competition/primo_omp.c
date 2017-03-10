#include <omp.h>
#include <math.h>

int ompPrime(long int n) {
    int i, in = 1;
    int sqrtn = sqrt(n)-2;

    if (n == 3) return 1;
    else if (n % 2 == 0 || n % 3 == 0) return 0;

#pragma omp parallel shared(sqrtn,n,in) private(i)
    {

    #pragma omp for schedule(dynamic,50) nowait
    for (i = 5; i <= sqrtn; i += 6) {
        if (n%i == 0 || n%(i+2) == 0) in=0;
    }

    }

    return in;

}
