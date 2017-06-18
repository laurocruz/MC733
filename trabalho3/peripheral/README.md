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
#include <string.h>

typedef union {
    uint32_t i;
    float f;
} Reading;

volatile Reading *na = (Reading *) 600000004U;
volatile Reading *nb = (Reading *) 600000008U;

volatile Reading *nanbsum = (Reading *) 600000020U;

float sum_float(float a, float b){
    float help;
    /* Pass the bits and not autocast it into int */
    (*na).i = *((uint32_t*) &a);
    (*nb).i = *((uint32_t*) &b);

    /* Here trying to get the value from *nanbsum */
    // para mim o correto seria - mas nao esta funcina

    help = *(float*)&*nanbsum;

    // O print disso aqui fica:
    // 1: 40bccccc 40bccccc 0 -0.000000
    // Sendo que 40bccccc é o 5.9 que é o resultado do float...
    // Mas eu nao consigo fazer ele "entrar" em uma variável do tipo float :(
    printf("1: %x %x %x %f\n", *nanbsum, *(float*)&(*nanbsum), help, help);

    return *((float*) &nanbsum);
}

int main(int argc, char *argv[]){
  int i;

  printf("Sum: %f", sum_float(3.1, 2.8));

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}
```
