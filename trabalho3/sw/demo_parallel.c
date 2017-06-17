#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// Memory size
#define MEM_SIZE 536870912U

// Addres of the lock peripherical
#define LOCK_ADDR (MEM_SIZE+1)

// Hardware lock
volatile uint32_t * lock = (uint32_t *) LOCK_ADDR;

volatile uint8_t set_lock = 0;
volatile uint8_t proc_lock = 0;
volatile uint8_t kf_lock = 0;

#define ACQUIRE_GLOBAL_LOCK() while(*lock)
#define RELEASE_GLOBAL_LOCK() *lock = 0

inline void acquireLock(volatile uint8_t * l) {
    int i;

    ACQUIRE_GLOBAL_LOCK();
    while (*l) {
        RELEASE_GLOBAL_LOCK();
        for(i = 0; i < 100; i++);
        ACQUIRE_GLOBAL_LOCK();
    }
    *l = 1;
    RELEASE_GLOBAL_LOCK();
}

inline void releaseLock(volatile uint8_t * l) { 
    *l = 0; 
}

volatile int processors = 0;
volatile uint8_t set = 0;
volatile uint8_t fft = 0;
volatile uint8_t kf = 0;

#include "kiss_fft_parallel.c"


// ----------------- Program variables -------------------- //
const int nfft = 65536;
//const int nfft = 1048576;
const int ndims = 1;
const int isinverse = 0;
const int numffts = 1;
int nbytes;

kiss_fft_cpx * buf; 
kiss_fft_cpx * bufout;
kiss_fft_cfg st;


int main(int argc, char ** argv) {
    int proc;
    int i, k;

    acquireLock(&proc_lock);
    proc = ++processors;
    releaseLock(&proc_lock);

    if (proc == 1) {
        nbytes = sizeof(kiss_fft_cpx) * nfft;

        buf = (kiss_fft_cpx *) malloc(nbytes);
        bufout = (kiss_fft_cpx *) calloc(1, nbytes);
    
        st = kiss_fft_alloc(nfft, isinverse, NULL, NULL);

        set = 1;
    }

    while (set == 0);

    // Processamento principal
    for (i = 0; i < numffts; ++i) {
        kiss_fft(st, buf, bufout, proc);

    }

    acquireLock(&set_lock);
    fft++;
    releaseLock(&set_lock);

    while (fft != processors);

    if (proc == 1) {
        free(st);

        fprintf(stderr, "KISS\tnfft=");
        fprintf(stderr, "%d,", nfft);
        fprintf(stderr, "\tnumffts=%d\n", numffts);

        free(buf); free(bufout);

        printf("FOI\n");

        fft = 0;
    }

    while (fft != 0);

    return 0;
}

