## Peripherical
Este é a implementação dos periféricos.
Existe o periférico de operações com float, em que operação com ponto flutuante gastam somente um único ciclo e operações trigonométricas

### Operações com float
Para fazer operações com float o funcionamento é o seguinte:
 - Os número `a` e `b` devem ser passados nos endereços `600000004U` e `600000008U` respectivamente.
 - O resultado das operações são passados nos seguintes endereços:
     - Soma: `600000020U`
     - Subtração: `600000030U`
     - Multiplicação: `600000040U`
     - Divisão: `600000050U`
Um exemplo de uso está abaixo

##### hello_world.c
Esse deve ser o conteudo do sw original
```
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdint.h>

typedef union {
    uint32_t i;
    float f;
} Reading;

volatile Reading *float_a = (Reading *) 600000004U;
volatile Reading *float_b = (Reading *) 600000008U;

volatile Reading *sum_fl = (Reading *) 600000020U;
volatile Reading *sub_fl = (Reading *) 600000030U;
volatile Reading *mul_fl = (Reading *) 600000040U;
volatile Reading *div_fl = (Reading *) 600000050U;
volatile Reading *sin_fl = (Reading *) 600000110U;
volatile Reading *cos_fl = (Reading *) 600000120U;

void sum_float(float a, float b, float* sum){
    /* Pass the bits and not autocast it into int */
    (*float_a).i = *((uint32_t*) &a);
    (*float_b).i = *((uint32_t*) &b);

    *sum = sum_fl->f;
}

void sub_float(float a, float b, float* sub){
    /* Pass the bits and not autocast it into int */
    (*float_a).i = *((uint32_t*) &a);
    (*float_b).i = *((uint32_t*) &b);

    *sub = sub_fl->f;
}

void mul_float(float a, float b, float* mul){
    /* Pass the bits and not autocast it into int */
    (*float_a).i = *((uint32_t*) &a);
    (*float_b).i = *((uint32_t*) &b);

    *mul = mul_fl->f;
}

void div_float(float a, float b, float* div_){
    /* Pass the bits and not autocast it into int */
    (*float_a).i = *((uint32_t*) &a);
    (*float_b).i = *((uint32_t*) &b);

    *div_ = div_fl->f;
}

void sin_float(float a, float* sin_){
    /* Pass the bits and not autocast it into int */
    (*float_a).i = *((uint32_t*) &a);

    *sin_ = sin_fl->f;
}

void cos_float(float a, float* cos_){
    /* Pass the bits and not autocast it into int */
    (*float_a).i = *((uint32_t*) &a);

    *cos_ = cos_fl->f;
}

int main(int argc, char *argv[]){
    int i;
    float sum, sub, mul, divi, sen, cose;

    sum_float(3.1, 2.8, &sum);
    sub_float(3.1, 3.0, &sub);
    mul_float(2.0, 2.5, &mul);
    div_float(7.75, 2.25, &divi);
    sin_float(3.1415, &sen);
    cos_float(3.1415, &cose);

    printf("Sum: %d\n", (int) (sum*1000));
    printf("Sub: %d\n", (int) (sub*1000));
    printf("Mul: %d\n", (int) (mul*1000));
    printf("Div: %d\n", (int) (divi*1000));
    printf("Sec: %d\n", (int) (sen*1000));
    printf("Cos: %d\n", (int) (cose*1000));

    exit(0); // To avoid cross-compiler exit routine
    return 0; // Never executed, just for compatibility
}
```
