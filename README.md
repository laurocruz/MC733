# MC733
Repositório para os exercícios propostos na aula de MC733 (Projeto de sistemas computacionais) - Professor Lucas Wanner

---

# Exercício 1

## Código original

Inicialmente foram feitos testes de tempo (usando o comando `time`) com o código oferecido no enunciado do exercício com apenas uma alteração, o número testado foi 104395301 (primo). O programa foi compilado separadamente usando as flags de otimização `-O0`, `-O1`, `-O2`, `-O3`.

Os resultados podes ser vistos abaixo:

* NO: 0.30s
* `-O0`: 0.30s
* `-O1`: 0.36s
* `-O2`: 0.37s
* `-O3`: 0.40s

Como podemos ver acima, os tempos de execução para as versões se, otimização e usando `-O0` foram as mais rápidas inicialmente, contrariando o que poderia se pensar que o tempo seria decrescente.

Nesta versão então foi também realizada uma compilação usando a flag `-mtune=native`, que realiza uma otimização específica para o processador para o qual está compilando (no caso do `native` o processador é definido em tempo de compilação.)

* `mtune=native`: 0.26s

Podemos ver acima que o tempo de execução foi menor do que o anteriormente mais raṕido.

## Makefile

Após a criação de um Makefile para compilar o código, ele também foi separado em dois arquivos (`main.c` e `calc_primo.c`). Um novo teste de tempo foi realizado com a compilação que gerão o menor tempo nos testes anteriores (`-mtune=native`). Um resultado maior de 0.34s foi obtido.

```
SOURCE=main.c calc_primo.c
OBJ=primo
FLAGS=-pg

mtune: ${SOURCE}
    gcc ${SOURCE} -o ${OBJ} ${FLAGS} -mtune=native
```

## Número de primos entre 1 e n

Uma das alterações realizadas no código fonte oferecido foi para ao invés de descobrir se um número é ou não primo, descobrir quantos primos existem entre 1 e um dado valor n. A alteração foi feita na main da seguinte forma:

```
for (int i = 2; i <= n; i++)
    if (primo(i)) x++;
```

Sendo `x` o contador de primos.

Para esse código, foram realizados testes de tempo da mesma forma como anteriormente, mas agora com o input n sendo 500000. Os testes foram feitos para o código com aruivos separados e juntos. Os valores para arquivos separados seguem abaixo:

* NO: 27.083s
* `-O0`: 27.403s
* `-O1`: 27.541s
* `-O2`: 29.516s
* `-O3`: 27.273s
* `mtune`: 26.740s

Pode-se perceber que o valor para a otimização da flag `mtune` é menor, como era esperado pelos valores anteriores.

Para arquivos juntos:

* NO: 30.643s
* `-O0`: 30.820s
* `-O1`: 28.583s
* `-O2`: 27.362s
* `-O3`: 27.673s
* `mtune`: 31.825s

Já neste caso temos uma inversão, os valores para as otimizações `-O2` e `-O3`, que eram as mais lentas nas medidas anteriores, são aqui as mais rápidas.

## GPROF

Foi utilizado o GNU Profiler (GProf) para realizar o profiling do algoritmo e analisar a função em que o programa mais gasta tempo. Como era esperado, essa função é a que descobre se o número é primo. Afinal no momento ela é chamada para todos os valores dentre 1 e n, e ela própria está muito mal implementada. Isso claramente implica que no processo de otimizar o programa, essa função deve ser o foco do trabalho.

## Otimização do programa para a competição

Para a competição o programa, ao invés de informar a quantidade de primos entre 1 e n deve retornar o valor do n-ésimo número primo, sendo n um valor inserido pelo usuário.

As duas principais otimizações realizadas foram:

### 1. Verificar se o número é divisível apenas pelos valores menores ou iguais a raiz quadrada do número

A primeiro otimização feita no programa a de verificar a multiplicidade apenas até sqrt(n).

### 2. Testar se o número é multiplo apenas dos valores primos menores que ele

Todo número natural é multiplo de primos (se esse número é primo, ele é múltiplo apenas dele mesmo). Ou seja, se o número não é múltiplo de 2 ou 5, é inútil testar se ele é multiplo de 10. Assim, se o valor não é múltiplo de nenhum primo que o precede, ele próprio deve ser um primo.

Como especificado no item anterior, o teste precisa ser feito apenas para os primos menores o iguais a sqrt(n).

Os primos já definidos são salvos em um vetor que é usado para determinar a primalidade do próximo primo.

#### Após estas alterações obtemos os seguintes valores reais (todos obtidos para o 500000-ésimo primo):

* NO: 4.001s
* `-O0`: 4.570s
* `-O1`: 5.031s
* `-O2`: 5.303s
* `-O3`: 4.570s
* `mtune`: 4.322s

## Paralelização

Para a paralelização foi usado a biblioteca OpenMP. Tentei paralelizar a função que testa a primalidade dos números usando a diretiva FOR. Infelizmente não foram obtidos bons valores nessa tentativa pois não é possível acabar com a execução das threads no meio, assim, no caso de valores não primos, apesar de uma thread descobrir que o valor é composto, a execução não para e continua-se a testar os valores restantes.

Segue a paralelização feita:

```
#pragma omp parallel shared(sqrtn,n,in) private(i)
{
    #pragma omp for schedule(dynamic,50) nowait
    for (i = 5; i <= sqrtn; i += 6) {
        if (n%i == 0 || n%(i+2) == 0) in=0;
    }
}
```

Os tempos obtidos foram:

* NO: 10.570s
* `-O0`: 11.101s
* `-O1`: 10.439s
* `-O2`: 11.135s
* `-O3`: 10.422s
* `mtune`: 10.580s

Pode-se perceber que os tempos ficaram bem mais lentos que a versão sem paralelização.
