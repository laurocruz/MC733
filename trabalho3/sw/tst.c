#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "kiss_fft.c"

static int getdims(int * dims, char * arg) {
    char *s;
    int ndims=0;
    while ( (s=strtok( arg , ",") ) ) {
        dims[ndims++] = atoi(s);
        //printf("%s=%d\n",s,dims[ndims-1]);
        arg=NULL;
    }
    return ndims;
}

int main(int argc, char ** argv) { 
    int k;
    int nfft[32];
    int ndims = 1;
    int isinverse=0;
    int numffts=1000,i;
    int nbytes;   
    int c = 0;
    kiss_fft_cfg cfg;
    kiss_fft_cpx * buf;
    kiss_fft_cpx * bufout;

    printf("COMEÇOU\n");

    nfft[0] = 1024; 

    while (1) {
        int c = getopt (argc, argv, "n:ix:");
        if (c == -1)
            break;
        switch (c) {
            case 'n':
                ndims = getdims(nfft, optarg );
                if (nfft[0] != kiss_fft_next_fast_size(nfft[0]) ) {
                    int ng = kiss_fft_next_fast_size(nfft[0]);
                    fprintf(stderr,"warning: %d might be a better choice for speed than %d\n",ng,nfft[0]);
                }
                break;
            case 'x':
                numffts = atoi (optarg);
                break;
            case 'i':
                isinverse = 1;
                break;
        }
    }

    //printf ("nfft[0] = %d\nnumffts = %d\n",nfft[0],numffts);

    nbytes = sizeof(kiss_fft_cpx);
    // PARALELIZAVEL (REDUCAO DA MULTIPLICACAO em nbytes)
    for (k=0;k<ndims;++k)
        nbytes *= nfft[k];
    printf("NBYTES\n");

    buf=(kiss_fft_cpx*)KISS_FFT_MALLOC(nbytes);
    bufout=(kiss_fft_cpx*)KISS_FFT_MALLOC(nbytes);
    memset(buf,0,nbytes);

    if (ndims==1) {
        kiss_fft_cfg st = kiss_fft_alloc(nfft[0], isinverse, NULL, NULL);
        printf("ALLOC\n");

        for (i=0;i<numffts;++i)
            kiss_fft( st ,buf,bufout );
        free(st);
    }


    fprintf(stderr,"KISS\tnfft=");
    for (k=0;k<ndims;++k)
        fprintf(stderr, "%d,",nfft[k]);
    fprintf(stderr,"\tnumffts=%d\n" ,numffts);

    free(buf); free(bufout);

    printf("FOI\n");
}   
