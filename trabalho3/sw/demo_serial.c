#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "kiss_fft_serial.c"

int main(int argc, char ** argv) {
    const int nfft = 65536;
    const int ndims = 1;
    const int isinverse = 0;
    const int numffts = 1;
    int nbytes;
    int i;
    kiss_fft_cpx * buf; 
    kiss_fft_cpx * bufout;
    kiss_fft_cfg st;

    nbytes = sizeof(kiss_fft_cpx) * nfft;

    buf = (kiss_fft_cpx *) malloc(nbytes);
    bufout = (kiss_fft_cpx *) calloc(1, nbytes);

    st = kiss_fft_alloc(nfft, isinverse, NULL, NULL);

    // Processamento principal
    for (i = 0; i < numffts; ++i) {
        kiss_fft(st, buf, bufout);
    }

    free(st);

    fprintf(stderr, "KISS\tnfft=");
    fprintf(stderr, "%d,", nfft);
    fprintf(stderr, "\tnumffts=%d\n", numffts);

    free(buf); free(bufout);

    printf("FOI\n");

    return 0;
}

