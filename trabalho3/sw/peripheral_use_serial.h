#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdint.h>

typedef union {
    uint32_t i;
    float f;
} Reading;

volatile Reading *float_a = (Reading *) 1992294100U;
volatile Reading *float_b = (Reading *) 1992294200U;

volatile Reading *sum_fl = (Reading *)  1992294300U;
volatile Reading *sub_fl = (Reading *)  1992294400U;
volatile Reading *mul_fl = (Reading *)  1992294500U;
volatile Reading *div_fl = (Reading *)  1992294600U;
volatile Reading *sin_fl = (Reading *)  1992294700U;
volatile Reading *cos_fl = (Reading *)  1992294800U;


float sum_float(float a, float b){
    /* Pass the bits and not autocast it into int */
    (*float_a).i = *((uint32_t*) &a);
    (*float_b).i = *((uint32_t*) &b);

    return sum_fl->f;
}

float sub_float(float a, float b){
    /* Pass the bits and not autocast it into int */
    (*float_a).i = *((uint32_t*) &a);
    (*float_b).i = *((uint32_t*) &b);

    return sub_fl->f;
}

float mul_float(float a, float b){
    /* Pass the bits and not autocast it into int */
    (*float_a).i = *((uint32_t*) &a);
    (*float_b).i = *((uint32_t*) &b);

    return mul_fl->f;
}

float div_float(float a, float b){
    /* Pass the bits and not autocast it into int */
    (*float_a).i = *((uint32_t*) &a);
    (*float_b).i = *((uint32_t*) &b);

    return div_fl->f;
}

float sin_float(float a){
    /* Pass the bits and not autocast it into int */
    (*float_a).i = *((uint32_t*) &a);

    return sin_fl->f;
}

float cos_float(float a){
    /* Pass the bits and not autocast it into int */
    (*float_a).i = *((uint32_t*) &a);

    return cos_fl->f;
}
