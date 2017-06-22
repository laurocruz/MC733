#ifndef _stdint_h_
#include <stdint.h>
#endif

#define N_A     600000004U
#define N_B     600000008U
#define SOMA    600000020U
#define SUB     600000030U
#define MULT    600000040U
#define DIV     600000050U
#define SEN     600000110U
#define COS     600000120U

/* This union help to convert int to float whithout casting it */
typedef union {
    uint32_t i;
    float f;
} Reading;
