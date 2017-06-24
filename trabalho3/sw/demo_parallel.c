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
volatile uint8_t alloc_lock = 0;

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
volatile uint8_t start_alloc = 0;
volatile uint8_t alloc = 0;
volatile uint8_t factor = 0;

#include "kiss_fft_parallel.c"


// ----------------- Program variables -------------------- //
#ifdef _NFFT_12
const int nfft = 65536;
#else
const int nfft = 1024;
#endif

const int ndims = 1;
const int isinverse = 0;

#ifdef _NUM_100
const int numffts = 10;
#elif _NUM_1000
const int numffts = 50;
#endif

int nbytes;

kiss_fft_cpx * buf;
kiss_fft_cpx * bufout;
kiss_fft_cfg main_st;
size_t st_memsize;


int main(int argc, char ** argv) {
    int proc;
    int i, k;

    acquireLock(&proc_lock);
    proc = ++processors;
    releaseLock(&proc_lock);
    //fprintf(stderr,"E");

    if (proc == 1) {
        nbytes = sizeof(kiss_fft_cpx) * nfft;

        buf = (kiss_fft_cpx *) malloc(nbytes);
        bufout = (kiss_fft_cpx *) calloc(1, nbytes);

        st_memsize = sizeof(struct kiss_fft_state)
            + sizeof(kiss_fft_cpx)*(nfft-1); /* twiddle factors*/

        main_st = ( kiss_fft_cfg)KISS_FFT_MALLOC( st_memsize );
        set = 1;
    }

    while (set == 0);

    kiss_fft_alloc(nfft, isinverse, main_st, &st_memsize, proc);

    acquireLock(&alloc_lock);
    alloc++;
    releaseLock(&alloc_lock);

    while(alloc != processors);

    // Processamento principal
    for (i = 0; i < numffts; ++i)
        kiss_fft(main_st, buf, bufout, proc);

    acquireLock(&set_lock);
    fft++;
    releaseLock(&set_lock);

    while (fft != processors);

    if (proc == 1) {
        free(main_st);

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
