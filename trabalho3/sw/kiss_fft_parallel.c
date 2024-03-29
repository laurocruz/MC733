/*
Copyright (c) 2003-2010, Mark Borgerding

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "_kiss_fft_guts_parallel.h"
/* The guts header contains all the multiplication and addition macros that are defined for
 fixed or floating point complex numbers.  It also delares the kf_ internal functions.
 */

/* This is for peripheral usage */
#include "peripheral_use_parallel.h"

//

#ifdef _PER
#define USE_PER 1
#else
#define USE_PER 0
#endif

static void kf_bfly2(
        kiss_fft_cpx * Fout,
        const size_t fstride,
        const kiss_fft_cfg st,
        int m,
        int proc
        )
{
    kiss_fft_cpx * Fout2;
    kiss_fft_cpx * tw1 = st->twiddles;
    kiss_fft_cpx t;


    Fout2 = Fout + m;

    do{
        C_FIXDIV(*Fout,2); C_FIXDIV(*Fout2,2);


#ifdef _PER
        C_MUL_P (t,  *Fout2 , *tw1, proc);

        tw1 += fstride;
        C_SUB_P( *Fout2 ,  *Fout , t , proc);
        C_ADDTO_P( *Fout ,  t, proc );
#else
        C_MUL (t,  *Fout2 , *tw1);

        tw1 += fstride;

        C_SUB( *Fout2 ,  *Fout , t );
        C_ADDTO( *Fout ,  t );
#endif

        ++Fout2;
        ++Fout;
    }while (--m);
}

static void kf_bfly4(
        kiss_fft_cpx * Fout,
        const size_t fstride,
        const kiss_fft_cfg st,
        const size_t m,
        int proc
        )
{
    kiss_fft_cpx *tw1,*tw2,*tw3;
    kiss_fft_cpx scratch[6];
    size_t k=m;
    const size_t m2=2*m;
    const size_t m3=3*m;


    tw3 = tw2 = tw1 = st->twiddles;

    do {
        C_FIXDIV(*Fout,4); C_FIXDIV(Fout[m],4); C_FIXDIV(Fout[m2],4); C_FIXDIV(Fout[m3],4);

#ifdef _PER
        C_MUL_P(scratch[0],Fout[m] , *tw1, proc);
        C_MUL_P(scratch[1],Fout[m2] , *tw2, proc);
        C_MUL_P(scratch[2],Fout[m3] , *tw3, proc);

        C_SUB_P( scratch[5] , *Fout, scratch[1] , proc);
        C_ADDTO_P(*Fout, scratch[1], proc);
        C_ADD_P( scratch[3] , scratch[0] , scratch[2] , proc);
        C_SUB_P( scratch[4] , scratch[0] , scratch[2] , proc);
        C_SUB_P( Fout[m2], *Fout, scratch[3] , proc);
#else
        C_MUL(scratch[0],Fout[m] , *tw1 );
        C_MUL(scratch[1],Fout[m2] , *tw2 );
        C_MUL(scratch[2],Fout[m3] , *tw3 );

        C_SUB( scratch[5] , *Fout, scratch[1] );
        C_ADDTO(*Fout, scratch[1]);
        C_ADD( scratch[3] , scratch[0] , scratch[2] );
        C_SUB( scratch[4] , scratch[0] , scratch[2] );
        C_SUB( Fout[m2], *Fout, scratch[3] );
#endif


        tw1 += fstride;
        tw2 += fstride*2;
        tw3 += fstride*3;

        C_ADDTO( *Fout , scratch[3] );

        if(st->inverse) {
#ifdef _PER
            Fout[m].r = sub_float(scratch[5].r, scratch[4].i, proc);
            Fout[m].i = sum_float(scratch[5].i, scratch[4].r, proc);
            Fout[m3].r = sum_float(scratch[5].r, scratch[4].i, proc);
            Fout[m3].i = sub_float(scratch[5].i, scratch[4].r, proc);
#else
            Fout[m].r = scratch[5].r - scratch[4].i;
            Fout[m].i = scratch[5].i + scratch[4].r;
            Fout[m3].r = scratch[5].r + scratch[4].i;
            Fout[m3].i = scratch[5].i - scratch[4].r;
#endif
        }else{
#ifdef _PER
            Fout[m].r = sum_float(scratch[5].r, scratch[4].i, proc);
            Fout[m].i = sub_float(scratch[5].i, scratch[4].r, proc);
            Fout[m3].r = sub_float(scratch[5].r, scratch[4].i, proc);
            Fout[m3].i = sum_float(scratch[5].i, scratch[4].r, proc);
#else
            Fout[m].r = scratch[5].r + scratch[4].i;
            Fout[m].i = scratch[5].i - scratch[4].r;
            Fout[m3].r = scratch[5].r - scratch[4].i;
            Fout[m3].i = scratch[5].i + scratch[4].r;
#endif
        }
        ++Fout;
    }while(--k);
}

static void kf_bfly3(
         kiss_fft_cpx * Fout,
         const size_t fstride,
         const kiss_fft_cfg st,
         size_t m,
         int proc
         )
{
     size_t k=m;
     const size_t m2 = 2*m;
     kiss_fft_cpx *tw1,*tw2;
     kiss_fft_cpx scratch[5];
     kiss_fft_cpx epi3;
     epi3 = st->twiddles[fstride*m];

     tw1=tw2=st->twiddles;

     do{
#ifdef _PER
        C_FIXDIV_P(*Fout,3, proc);
        C_FIXDIV_P(Fout[m],3, proc);
        C_FIXDIV_P(Fout[m2],3, proc);

        C_MUL_P(scratch[1],Fout[m] , *tw1, proc);
        C_MUL_P(scratch[2],Fout[m2] , *tw2, proc);

        C_ADD_P(scratch[3],scratch[1],scratch[2], proc);
        C_SUB_P(scratch[0],scratch[1],scratch[2], proc);

        tw1 += fstride;
        tw2 += fstride*2;

        Fout[m].r = sub_float(Fout->r, HALF_OF(scratch[3].r), proc);
        Fout[m].i = sub_float(Fout->i, HALF_OF(scratch[3].i), proc);
#else
        C_FIXDIV(*Fout,3);
        C_FIXDIV(Fout[m],3);
        C_FIXDIV(Fout[m2],3);

        C_MUL(scratch[1],Fout[m] , *tw1);
        C_MUL(scratch[2],Fout[m2] , *tw2);

        C_ADD(scratch[3],scratch[1],scratch[2]);
        C_SUB(scratch[0],scratch[1],scratch[2]);

        tw1 += fstride;
        tw2 += fstride*2;

        Fout[m].r = Fout->r - HALF_OF(scratch[3].r);
        Fout[m].i = Fout->i - HALF_OF(scratch[3].i);
#endif


#ifdef _PER
        C_MULBYSCALAR_P( scratch[0] , epi3.i, proc);

        C_ADDTO_P(*Fout,scratch[3], proc);

        Fout[m2].r = sum_float(Fout[m].r, scratch[0].i, proc);
        Fout[m2].i = sub_float(Fout[m].i, scratch[0].r, proc);

        Fout[m].r = sum_float(Fout[m].r, scratch[0].i, proc);
        Fout[m].i = sub_float(Fout[m].i, scratch[0].r, proc);
#else
        C_MULBYSCALAR( scratch[0] , epi3.i );

        C_ADDTO(*Fout,scratch[3]);

        Fout[m2].r = Fout[m].r + scratch[0].i;
        Fout[m2].i = Fout[m].i - scratch[0].r;

        Fout[m].r  -= scratch[0].i;
        Fout[m].i  += scratch[0].r;
#endif
         ++Fout;
     }while(--k);
}

static void kf_bfly5(
        kiss_fft_cpx * Fout,
        const size_t fstride,
        const kiss_fft_cfg st,
        int m,
        int proc
        )
{
    kiss_fft_cpx *Fout0,*Fout1,*Fout2,*Fout3,*Fout4;
    int u;
    kiss_fft_cpx scratch[13];
    kiss_fft_cpx * twiddles = st->twiddles;
    kiss_fft_cpx *tw;
    kiss_fft_cpx ya,yb;
    ya = twiddles[fstride*m];
    yb = twiddles[fstride*2*m];

    Fout0=Fout;
    Fout1=Fout0+m;
    Fout2=Fout0+2*m;
    Fout3=Fout0+3*m;
    Fout4=Fout0+4*m;

    tw=st->twiddles;
    for ( u=0; u<m; ++u ) {
#ifdef _PER
        C_FIXDIV_P( *Fout0,5,proc);
        C_FIXDIV_P( *Fout1,5,proc);
        C_FIXDIV_P( *Fout2,5,proc);
        C_FIXDIV_P( *Fout3,5,proc);
        C_FIXDIV_P( *Fout4,5,proc);

        scratch[0] = *Fout0;

        C_MUL_P(scratch[1] ,*Fout1, tw[u*fstride], proc);
        C_MUL_P(scratch[2] ,*Fout2, tw[2*u*fstride], proc);
        C_MUL_P(scratch[3] ,*Fout3, tw[3*u*fstride], proc);
        C_MUL_P(scratch[4] ,*Fout4, tw[4*u*fstride], proc);

        C_ADD_P( scratch[7],scratch[1],scratch[4], proc);
        C_SUB_P( scratch[10],scratch[1],scratch[4], proc);
        C_ADD_P( scratch[8],scratch[2],scratch[3], proc);
        C_SUB_P( scratch[9],scratch[2],scratch[3], proc);

        Fout0->r = sum_float(scratch[7].r, scratch[8].r, proc);
        Fout0->i = sum_float(scratch[7].i, scratch[8].i, proc);

        scratch[5].r = sum_float(scratch[0].r, S_MUL(scratch[7].r,ya.r), proc);
        scratch[5].r = sum_float(scratch[5].r, S_MUL(scratch[8].r,yb.r), proc);
        scratch[5].i = sum_float(scratch[0].i, S_MUL(scratch[7].r,ya.r), proc);
        scratch[5].i = sum_float(scratch[5].i, S_MUL(scratch[8].i,yb.r), proc);

        scratch[6].r = sum_float(S_MUL(scratch[10].i,ya.i), S_MUL(scratch[9].i,yb.i), proc);
        scratch[6].i = sub_float(-S_MUL(scratch[10].r,ya.i), S_MUL(scratch[9].r,yb.i), proc);
#else
        C_FIXDIV( *Fout0,5);
        C_FIXDIV( *Fout1,5);
        C_FIXDIV( *Fout2,5);
        C_FIXDIV( *Fout3,5);
        C_FIXDIV( *Fout4,5);

        scratch[0] = *Fout0;

        C_MUL(scratch[1] ,*Fout1, tw[u*fstride]);
        C_MUL(scratch[2] ,*Fout2, tw[2*u*fstride]);
        C_MUL(scratch[3] ,*Fout3, tw[3*u*fstride]);
        C_MUL(scratch[4] ,*Fout4, tw[4*u*fstride]);

        C_ADD( scratch[7],scratch[1],scratch[4]);
        C_SUB( scratch[10],scratch[1],scratch[4]);
        C_ADD( scratch[8],scratch[2],scratch[3]);
        C_SUB( scratch[9],scratch[2],scratch[3]);

        Fout0->r += scratch[7].r + scratch[8].r;
        Fout0->i += scratch[7].i + scratch[8].i;

        scratch[5].r = scratch[0].r + S_MUL(scratch[7].r,ya.r) + S_MUL(scratch[8].r,yb.r);
        scratch[5].i = scratch[0].i + S_MUL(scratch[7].i,ya.r) + S_MUL(scratch[8].i,yb.r);

        scratch[6].r =  S_MUL(scratch[10].i,ya.i) + S_MUL(scratch[9].i,yb.i);
        scratch[6].i = -S_MUL(scratch[10].r,ya.i) - S_MUL(scratch[9].r,yb.i);
#endif

        C_SUB(*Fout1,scratch[5],scratch[6]);
        C_ADD(*Fout4,scratch[5],scratch[6]);

#ifdef _PER
        scratch[11].r = sum_float(scratch[0].r, S_MUL(scratch[7].r,yb.r), proc);
        scratch[11].r = sum_float(scratch[11].r, S_MUL(scratch[8].r,ya.r), proc);
        scratch[11].i = sum_float(scratch[0].i, S_MUL(scratch[7].i,yb.r), proc);
        scratch[11].i = sum_float(scratch[11].i, S_MUL(scratch[8].i,ya.r), proc);

        scratch[12].r = sum_float(-S_MUL(scratch[10].i,yb.i), S_MUL(scratch[9].i,ya.i), proc);
        scratch[12].i = sub_float(S_MUL(scratch[10].r,yb.i), S_MUL(scratch[9].r,ya.i), proc);

        C_ADD_P(*Fout2,scratch[11],scratch[12], proc);
        C_SUB_P(*Fout3,scratch[11],scratch[12], proc);
#else
        scratch[11].r = scratch[0].r + S_MUL(scratch[7].r,yb.r) + S_MUL(scratch[8].r,ya.r);
        scratch[11].i = scratch[0].i + S_MUL(scratch[7].i,yb.r) + S_MUL(scratch[8].i,ya.r);

        scratch[12].r = - S_MUL(scratch[10].i,yb.i) + S_MUL(scratch[9].i,ya.i);
        scratch[12].i = S_MUL(scratch[10].r,yb.i) - S_MUL(scratch[9].r,ya.i);

        C_ADD(*Fout2,scratch[11],scratch[12]);
        C_SUB(*Fout3,scratch[11],scratch[12]);
#endif

        ++Fout0;++Fout1;++Fout2;++Fout3;++Fout4;
    }
}

/* perform the butterfly for one stage of a mixed radix FFT */
static void kf_bfly_generic(
        kiss_fft_cpx * Fout,
        const size_t fstride,
        const kiss_fft_cfg st,
        int m,
        int p
        )
{
    int u,k,q1,q;
    kiss_fft_cpx * twiddles = st->twiddles;
    kiss_fft_cpx t;
    int Norig = st->nfft;

    kiss_fft_cpx * scratch = (kiss_fft_cpx*)KISS_FFT_TMP_ALLOC(sizeof(kiss_fft_cpx)*p);

    for ( u=0; u<m; ++u ) {
        k=u;
        for ( q1=0 ; q1<p ; ++q1 ) {
            scratch[q1] = Fout[ k  ];
            C_FIXDIV(scratch[q1],p);
            k += m;
        }

        k=u;
        for ( q1=0 ; q1<p ; ++q1 ) {
            int twidx=0;
            Fout[ k ] = scratch[0];
            for (q=1;q<p;++q ) {
                twidx += fstride * k;
                if (twidx>=Norig) twidx-=Norig;
                C_MUL(t,scratch[q] , twiddles[twidx] );
                C_ADDTO( Fout[ k ] ,t);
            }
            k += m;
        }
    }
    KISS_FFT_TMP_FREE(scratch);
}

static
void kf_work(
        kiss_fft_cpx * Fout,
        const kiss_fft_cpx * f,
        const size_t fstride,
        int in_stride,
        int * factors,
        const kiss_fft_cfg st,
        int proc
        )
{
    kiss_fft_cpx * Fout_beg=Fout;
    const int p=*factors++; /* the radix  */
    const int m=*factors++; /* stage's fft length/p */
    const kiss_fft_cpx * Fout_end = Fout + p*m;


    // use openmp extensions at the
    // top-level (not recursive)
    if (fstride==1 && p<=5)
    {
        int k;

        // execute the p different work units in different threads
        for (k = proc-1; k < p; k += processors)
            kf_work( Fout +k*m, f+ fstride*in_stride*k,fstride*p,in_stride,factors,st, proc);
        // all threads have joined by this point

        acquireLock(&kf_lock);
        kf++;
        releaseLock(&kf_lock);

        while (kf != processors);

        if (proc == 1) {
            switch (p) {
                case 2: kf_bfly2(Fout,fstride,st,m,proc); break;
                case 3: kf_bfly3(Fout,fstride,st,m,proc); break;
                case 4: kf_bfly4(Fout,fstride,st,m,proc); break;
                case 5: kf_bfly5(Fout,fstride,st,m,proc); break;
                default: kf_bfly_generic(Fout,fstride,st,m,p); break;
            }
            kf = 0;
        }

        while (kf != 0);

        return;
    }

    if (m==1) {
        do{
            *Fout = *f;
            f += fstride*in_stride;
        }while(++Fout != Fout_end );
    }else{
        do{
            // recursive call:
            // DFT of size m*p performed by doing
            // p instances of smaller DFTs of size m,
            // each one takes a decimated version of the input
            kf_work( Fout , f, fstride*p, in_stride, factors,st,proc);
            f += fstride*in_stride;
        }while( (Fout += m) != Fout_end );
    }

    Fout=Fout_beg;

    // recombine the p smaller DFTs
    switch (p) {
        case 2: kf_bfly2(Fout,fstride,st,m, proc); break;
        case 3: kf_bfly3(Fout,fstride,st,m, proc); break;
        case 4: kf_bfly4(Fout,fstride,st,m, proc); break;
        case 5: kf_bfly5(Fout,fstride,st,m, proc); break;
        default: kf_bfly_generic(Fout,fstride,st,m,p); break;
    }
}

/*  facbuf is populated by p1,m1,p2,m2, ...
    where
    p[i] * m[i] = m[i-1]
    m0 = n                  */
static
void kf_factor(int n,int * facbuf)
{
    int p=4;
    double floor_sqrt;
    floor_sqrt = floor( sqrt((double)n) );

    /*factor out powers of 4, powers of 2, then any remaining primes */
    do {
        while (n % p) {
            switch (p) {
                case 4: p = 2; break;
                case 2: p = 3; break;
                default: p += 2; break;
            }
            if (p > floor_sqrt)
                p = n;          /* no more factors, skip to end */
        }
        n /= p;
        *facbuf++ = p;
        *facbuf++ = n;
    } while (n > 1);
}

/*
 *
 * User-callable function to allocate all necessary storage space for the fft.
 *
 * The return value is a contiguous block of memory, allocated with malloc.  As such,
 * It can be freed with free(), rather than a kiss_fft-specific function.
 * */
kiss_fft_cfg kiss_fft_alloc(int nfft,int inverse_fft,void * mem,size_t * lenmem , uint8_t proc)
{
    kiss_fft_cfg st = (kiss_fft_cfg)mem;

    if (proc == 1) {

        st->nfft=nfft;
        st->inverse = inverse_fft;

        start_alloc = 1;
    }

    while (start_alloc == 0);

    int i;
    int start = (proc-1)*(nfft/processors);
    int end;

    if (proc == processors)
        end = nfft;
    else end = proc*(nfft/processors);

    for (i=start;i<end;++i) {
        const double pi=3.141592653589793238462643383279502884197169399375105820974944;
        double phase = -2*pi*i / nfft;
        if (st->inverse)
            phase *= -1;
        kf_cexp(st->twiddles+i, phase );
    }

    if (proc == 1) {
        kf_factor(nfft,st->factors);
        factor = 1;
    }

    while (factor == 0);

    return st;
}


void kiss_fft_stride(kiss_fft_cfg st,const kiss_fft_cpx *fin,kiss_fft_cpx *fout,int in_stride,int proc)
{
    if (fin == fout) {
        //NOTE: this is not really an in-place FFT algorithm.
        //It just performs an out-of-place FFT into a temp buffer
        kiss_fft_cpx * tmpbuf = (kiss_fft_cpx*)KISS_FFT_TMP_ALLOC( sizeof(kiss_fft_cpx)*st->nfft);
        kf_work(tmpbuf,fin,1,in_stride, st->factors,st,proc);
        memcpy(fout,tmpbuf,sizeof(kiss_fft_cpx)*st->nfft);
        KISS_FFT_TMP_FREE(tmpbuf);
    }else{
        kf_work( fout, fin, 1,in_stride, st->factors,st,proc);
    }
}

void kiss_fft(kiss_fft_cfg cfg,const kiss_fft_cpx *fin,kiss_fft_cpx *fout,int proc)
{
    kiss_fft_stride(cfg,fin,fout,1,proc);
}


void kiss_fft_cleanup(void)
{
    // nothing needed any more
}

int kiss_fft_next_fast_size(int n)
{
    while(1) {
        int m=n;
        while ( (m%2) == 0 ) m/=2;
        while ( (m%3) == 0 ) m/=3;
        while ( (m%5) == 0 ) m/=5;
        if (m<=1)
            break; /* n is completely factorable by twos, threes, and fives */
        n++;
    }
    return n;
}
