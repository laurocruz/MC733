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
#define ACQUIRE_LOCK
#define RELEASE_LOCK

volatile int gproc = 0;

volatile int finish = 0;
volatile int read_b = 0;

int n;
int vector[100000];
unsigned long int global_sum = 0;

int main(int argc, char *argv[]){
    int proc;
    int i;
    int start, end;
    unsigned long int sum = 0;

    ACQUIRE_GLOBAL_LOCK();
    gproc++;
    proc = gproc;

    if (read_b == 0) {
        read_b = 1;
        scanf("%d", &n);

        for (i = 0; i < n; i++)
            scanf("%lu", &vector[i]);
    }
    RELEASE_GLOBAL_LOCK();

    start = (proc-1)*(n/2);
    end = proc*(n/2);

    for (i = start; i < end; i++)
        sum += vector[i];

    ACQUIRE_GLOBAL_LOCK();
    global_sum += sum;
    finish++;
    RELEASE_GLOBAL_LOCK();

    // BARRIER
    if (finish == 2) {
        if (*lock == 0) {
            if (read_b == 1) {
                read_b = 0;
                printf("Soma = %lu\n", global_sum);
            }
            RELEASE_GLOBAL_LOCK();
        }
    }


    /*
       printf("Initial read: lock = %d\n", *lock);
       printf("Second read: lock = %d\n", *lock);
       printf("Write 0\n");
     *lock = 0;
     printf("After write: lock = %d\n", *lock);
     printf("After previous read: lock = %d\n", *lock);
     */

    //printf("Peripheral value pre-write = %x\n", *lock);
    //printf("Peripheral value pre-read = %x\n", *lock);
    //printf("Peripheral value pos-read = %d\n", *lock);
    //printf("Peripheral address = %x\n", PER_ADDR);
    //printf("Mem size = %x\n", MEM_SIZE);

    //exit(0); // To avoid cross-compiler exit routine
    return 0; // Never executed, just for compatibility
}

