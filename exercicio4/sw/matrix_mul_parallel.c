#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// Memory size
#define MEM_SIZE 536870912U
// Addres of the peripherical
#define PER_ADDR (MEM_SIZE+1)

// Hardware lock
volatile uint32_t * lock = (uint32_t *) PER_ADDR;

#define ACQUIRE_GLOBAL_LOCK() while(*lock)
#define RELEASE_GLOBAL_LOCK() *lock = 0

volatile int processors = 0;
volatile int read_matrix = 0;
volatile int finish = 0;

int n;
long int a[1000000], b[1000000], c[1000000];

int main(int argc, char *argv[]) {
    int proc;
    int i, j, k;
    int start, end;

    ACQUIRE_GLOBAL_LOCK();
    processors++;
    proc = processors;

    if (read_matrix == 0) {
        read_matrix = 1;
        scanf("%d", &n);

        for(i = 0; i < n*n; i++)
            scanf("%ld", &a[i]);

        for(i = 0; i < n*n; i++)
            scanf("%ld", &b[i]);
    }
    RELEASE_GLOBAL_LOCK();

    start = (proc-1)*(n/2);
    end = proc*(n/2);

    for (i = start; i < end; i++) {
        for (j = 0; j < n; j++) {
            c[i*n + j] = 0;
            for (k = 0; k < n; k++) {
                c[i*n + j] += a[i*n + k] * b[k*n + j];
            }
        }
    }

    /*
    ACQUIRE_GLOBAL_LOCK();
    finish++;
    RELEASE_GLOBAL_LOCK();

    if (finish == 2) {
        if (*lock == 0) {
            if (read_matrix == 1) {
                read_matrix = 0;
                for (i = 0; i < n; i++) {
                    for (j = 0; j < n; j++) {
                        printf("%5d ", c[i*n+j]);
                    }
                    printf("\n");
                }
            }
            RELEASE_GLOBAL_LOCK();
        }
    }
    */

    return 0;
}


