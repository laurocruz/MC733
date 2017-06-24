#ifndef _stdint_h_
#include <stdint.h>
#endif

#define MEM_SIZE 536870912U

#define N_A     1992294100U
#define N_B     1992294200U
#define SUM     1992294300U
#define SUB     1992294400U
#define MUL     1992294500U
#define DIV     1992294600U
#define SEN     1992294700U
#define COS     1992294800U

/* This union help to convert int to float whithout casting it */
typedef union {
    uint32_t i;
    float f;
} Reading;
