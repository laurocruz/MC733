#ifndef _stdint_h_
#include <stdint.h>
#endif

#define MEM_SIZE 536870912U

#define N_A     1992294400U
#define N_B     1992294404U
#define SOMA    1992294408U
#define SUB     1992294412U
#define MULT    1992294416U
#define DIV     1992294420U
#define SEN     1992294424U
#define COS     1992294428U

/* This union help to convert int to float whithout casting it */
typedef union {
    uint32_t i;
    float f;
} Reading;
