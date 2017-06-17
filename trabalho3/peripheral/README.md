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
Um exemplo de uso está abaixo: (Não está funcionando a parte pegar o valor calculado acessado e converter para float... - se conseguir...)

##### hello_world.c
Esse deve ser o conteudo do sw original
```
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

volatile int *na = (int *) 600000004U;
volatile int *nb = (int *) 600000008U;

volatile int *nanbsum = (int *) 600000020U;

float sum_float(float a, float b){
    float sum;
    /* Pass the bits and not autocast it into int */
    *na = *((uint32_t*) &a);
    *nb = *((uint32_t*) &b);


    /* Here trying to get the value from *nanbsum */
    sum = *nanbsum;
    // para mim o correto seria - mas nao esta funcina
    // sum = *((float*) &(*nanbsum));
    
    printf("1: %x %x\n", sum, &(*nanbsum));
    printf("2: %x %x\n", sum, *nanbsum);

    printf("Em hex: %x Em float: %f\n", sum, sum);
    return sum;
}

int main(int argc, char *argv[]){
  int i;

  printf("Sum: %f", sum_float(3.1, 2.8));

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}
```
