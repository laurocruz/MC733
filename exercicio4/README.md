### Lauro Cruz e Souza - 156175

# Exercício 4: Plataformas e Periféricos

## Periférico

O periférico inicialmente presente no MIPS oferecido para realizar a tarefa apenas imprimia o endereço do periférico e o dado a ser escrito no caso da escrita e retornava sempre 0 em caso de leitura. Podemos ver esse exemplo na saída abaixo, em que foi feita, nesta ordem, uma escrita do valor 0xFF e uma leitura:

```
ArchC: -------------------- Starting Simulation --------------------

addr: 20000001, data: ff000000
Peripheral value = 0

ArchC: -------------------- Simulation Finished --------------------

```

Como a memória do simulador tem 512M, sua última posição é `0x20000000`, assim foi escolhida a posição `0x20000001` como a posição do periférico (que é acessado para qualquer endereço maior ou igual a `0x20000000`).

### Periférico como uma posição comum de memória

Uma primeira alteração feita para testar o periférico foi utilizá-lo como uma posição comum de memória, ou seja, armazenando o dado escrito e apenas retornando-o na leitura.

Para manter o estado do periférico, dentro do arquivo que o descreve foi adicionada uma variável global que armaneza seu valor.

O código para esta funcionalidade pode ser visto abaixo:

```c
uint32_t int_mem = 0;

ac_tlm_rsp_status ac_tlm_peripheral::writem( const uint32_t &a , const uint32_t &d )
{
    if (a == MEM_SIZE+1) {
        int_mem = (uint32_t) d;
        cout << "writem:" << endl;
        cout << "addr: " << std::hex << a << ", data: " << d << endl;
        return SUCCESS;
    }

    cout << "Peripheral ERROR: Address value\n" << endl;
    return ERROR;
}

ac_tlm_rsp_status ac_tlm_peripheral::readm( const uint32_t &a , uint32_t &d )
{
    if (a == MEM_SIZE+1) {
        *((uint32_t *) &d) = int_mem;
        cout << "readm:" << endl;
        cout << "addr: " << std::hex << a << ", data: " << d << endl;
        return SUCCESS;
    }

    cout << "Peripheral ERROR: Address value\n" << endl;
    return ERROR;
}

```

Abaixo temos a saída responsável por uma escrita e uma leitura no periférico:
```
ArchC: -------------------- Starting Simulation --------------------

writem:
addr: 20000001, data: ff000000
readm:
addr: 20000001, data: ff000000

Peripheral value = ff

ArchC: -------------------- Simulation Finished --------------------

```

Como é possível ver no períferico, a posição de memória que ele armazena tem 32 bits, ou seja, o ponteiro utilizado para acessar o periférica precisa ser referente a um tipo de dados de 32 bits.

A utilização do periférico também só é possível se acessada na posição `MEM_SIZE+1`, sendo `MEM_SIZE` o tamanho da memória, que aqui tem valor `536870912U`, se não a operação não será realizada e o periférico enviará uma mensagem de erro.

Um exemplo de utilização correta segue abaixo:

```c
// Memory size
#define MEM_SIZE 536870912U
// Addres of the peripherical
#define PER_ADDR (MEM_SIZE+1)

volatile uint32_t * peripherical = (uint32_t *) PER_ADDR;
```

### Periférico como hardware lock (increment and load)

Aqui implementamos o periférico com a funcionalidade esperada para o exercício. Não explicaremos seu funcionamento, já que este já esta detalhado no enunciado do exercício.

Implementação:

```c
uint32_t int_mem = 0;

ac_tlm_rsp_status ac_tlm_peripheral::writem( const uint32_t &a , const uint32_t &d )
{
    if (a == MEM_SIZE+1) {
        int_mem = (uint32_t) d;
        return SUCCESS;
    }

    cout << "Peripheral ERROR: Address value\n" << endl;
    return ERROR;
}

ac_tlm_rsp_status ac_tlm_peripheral::readm( const uint32_t &a , uint32_t &d )
{
    if (a == MEM_SIZE+1) {
        *((uint32_t *) &d) = int_mem;
        // writing 1 to the value (the peripheral messes up the value)
        int_mem = 0x01000000;

        return SUCCESS;
    }
    cout << "Peripheral ERROR: Address value\n" << endl;
    return ERROR;
}
```

A saída de um pequeno programa feito para avaliar o periférico segue abaixo:

```
ArchC: -------------------- Starting Simulation --------------------

Initial read: *lock = 0
Second read: *lock = 1
Write 0 to lock
Third read: *lock = 0
Forth read: *lock = 1

ArchC: -------------------- Simulation Finished --------------------
```

Acima, lock é o ponteiro para o periférico e é realizado, em sequência, duas leituras, uma escrita de 0, e mais duas leituras.

É possível ver pela saída que o resultado é o esperado.


## Plataforma Multicore

Para simular uma máquina com múltiplos processadores, dentro do arquivo `main.cpp` do MIPS, onde são instanciados os processadores, memória, periférico e bus, no qual os componentes do sistemas são interligados.

Segue abaixo um trexo do main.cpp que mostra a instanciação dos componentes:

```c
int sc_main(int ac, char *av[])
{
    .
    .
    .

    //!  ISA simulator
    mips mips_proc1("mips1");
    mips mips_proc2("mips2");

    //! Bus
    ac_tlm_bus bus("bus");
    // Memory
    ac_tlm_mem mem("mem");
    // Peripheral
    ac_tlm_peripheral peripheral("peripheral");

    mips_proc1.DM(bus.target_export);
    mips_proc2.DM(bus.target_export);
    bus.MEM_port(mem.target_export);
    bus.PERIPHERAL_port(peripheral.target_export);

    mips_proc1.init(ac, av);
    mips_proc1.set_prog_args();
    cerr << endl;

    mips_proc2.init(ac2, av2);
    mips_proc2.set_prog_args();
    cerr << endl;

    sc_start();

    .
    .
    .
}
```

### Multiplicação de matrizes

A aplicação desenvolvidade para testar o funcionamento dos processadores em um programa de execução paralela foi o problema da multiplicação de matrizes quadradas.

#### Versão paralela

Para fazer o programa, foi utilizado o periférico do lock para gerenciar a concorrência dos processadores.

No programa, um dos processadores faz a leitura das matrizes e então cada processador realiza o processamento para uma metade da matriz (dividida no laço das linhas).

O resultado final não é impresso já que as matrizes utilizadas no teste abaixo são grandes (200x200).

Abaixo temos a seção do código que realiza a divisão de trabalhos e o processamento:

```c
#define ACQUIRE_GLOBAL_LOCK() while(*lock)
#define RELEASE_GLOBAL_LOCK() *lock = 0

volatile int processors = 0;
volatile int read_matrix = 0;

int main() {
    .
    .
    .
    ACQUIRE_GLOBAL_LOCK();
    processors++;
    proc = processors;

    if (read_matrix == 0) {
        read_matrix = 1;
        scanf("%d", &n);

        for(i = 0; i < n*n; i++)
            scanf("%ld", &a[i]);

        for(i = 0; i < n*n; i++)
            scanf("%ld", &b[i]);
    }
    RELEASE_GLOBAL_LOCK();

    start = (proc-1)*(n/2);
    end = proc*(n/2);

    for (i = start; i < end; i++) {
        for (j = 0; j < n; j++) {
            c[i*n + j] = 0;
            for (k = 0; k < n; k++) {
                c[i*n + j] += a[i*n + k] * b[k*n + j];
            }
        }
    }
    .
    .
    .
}
```

O tempo de execução obtido:

```
ArchC: Simulation statistics
    Times: 31.92 user, 0.11 system, 30.64 real
    Number of instructions executed: 337889156
    Simulation speed: 10585.50 K instr/s
ArchC: Simulation statistics
    Times: 31.92 user, 0.11 system, 30.57 real
    Number of instructions executed: 367408425
    Simulation speed: 11510.29 K instr/s
```

#### Versão serial

Nesta versão utilizamos apenas um processador (condifuração original do MIPS) e a foi feito um programa comum da multiplicação de matrizes quadradas (que não será representado aqui devido a sua simplicidade). Tivemos o seguinte tempo de execução:

```
Info: /OSCI/SystemC: Simulation stopped by user.
ArchC: Simulation statistics
    Times: 28.99 user, 0.12 system, 27.73 real
    Number of instructions executed: 618611013
    Simulation speed: 21338.77 K instr/s
```

#### Comparação

É possível perceber com os dados acima que a versão não paralelizada do programa obteve resultados um pouco melhores (em torno de 9.5%). A explicação disso é que estamos **simulando** um sistema multicore com processamento paralelo, mas na prática toda essa simulação ocorre em um único processo e thread em nossa máquina (máquina base). Assim, apesar da simulação de paralelismo, toda a execução é verdadeiramente serial.

Assim, a versão "paralela" se torna um pouco mais lenta já que temos também o overhead causado pelo gerenciamento da concorrência dos processadores (utilizando o periférico de lock).

## Conclusão

Utilizando o simulador do MIPS podemos realizar uma boa simulação de paralelismo com multicores no quesito de funcionamento, uma vez que mesmo a execução do simulador sendo feita inteiramente em um único core, o gerenciamento da concorrência do programa no simulador precisa ser feita exatamente como seria em um sistema realmente paralelo.

Agora, para avaliar como a paralelização afeta o tempo de execução, o simulador não é capaz de nos oferecer informações relevantes, uma vez que o próprio código do simulador não é paralelizado.
