### Lauro Cruz e Souza - 156175

# Exercício 3 - Contagem de ciclos

## Contando instruções

Nesta fase foi feita a edição do arquivo `mips_isa.cpp` que descreve o funcionamento das instruções do mips para contar quantas vezes a instrução `add` é chamada pelo código `hello.c`:

```
#include <stdio.h>

int main() {
    printf("Hello, world!");
    return 0;
}
```

As alterações feitas foram:

Criação de uma variável global:
```
int n_adds;
```

Inicialização desta na função `void ac_behavior(begin)`:
```
void ac_behavior(begin)
{
    n_adds = 0;
    .
    .
    .
}
```

Incrementação da variável em `void ac_behavior(add)`:
```
void ac_behavior(begin)
{
    n_add++;
    .
    .
    .
}
```

Impressão o valor final da variável em `void ac_behavior(end)`
```
void ac_behavior(end)
{
    dbg_printf("Número de adds: %d\n", n_adds);
    dbg_printf("@@@ end behavior @@@\n");
}
```

O resultado obtido foi de uma utilização da instrução `add`. Inicialmente espera mais utilizações, mas analisando o assembly do executável, podemos ver que são utilizadas muitas vezes a instrução `addu`, que tem a mesma função de `add`, mas sem avaliar se houve overflow. A função `add` avalia se houve overflow dos registradores e chama uma exception no Sistema Operacional, sendo assim ela é usada apenas em casos específicos.

## Avaliação de desempenho

### Hello, world!

Utilizando a flag de estatísticas `-s` do simulador do mips, podemos obter as informações das instruções mais facilmente:

Número total de instruções:
```
[ArchC 2.1] Printing GLOBAL statistics from processor module mips:
     INSTRUCTIONS : 5352
     SYSCALLS : 19
```

Instruções de acesso à memória:
```
[ArchC 2.1] Printing statistics from instruction lb:
     COUNT : 62
[ArchC 2.1] Printing statistics from instruction lbu:
     COUNT : 33
[ArchC 2.1] Printing statistics from instruction lh:
     COUNT : 26
[ArchC 2.1] Printing statistics from instruction lhu:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction lw:
     COUNT : 807
[ArchC 2.1] Printing statistics from instruction lwl:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction lwr:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction sb:
     COUNT : 62
[ArchC 2.1] Printing statistics from instruction sh:
     COUNT : 17
[ArchC 2.1] Printing statistics from instruction sw:
     COUNT : 558
[ArchC 2.1] Printing statistics from instruction swl:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction swr:
     COUNT : 0

Total = 1565 instruções
```

Instrucões de controle (branch/jump):
```
[ArchC 2.1] Printing statistics from instruction j:
     COUNT : 48
[ArchC 2.1] Printing statistics from instruction jal:
     COUNT : 75
[ArchC 2.1] Printing statistics from instruction jr:
     COUNT : 77
[ArchC 2.1] Printing statistics from instruction jalr:
     COUNT : 1
[ArchC 2.1] Printing statistics from instruction beq:
     COUNT : 213
[ArchC 2.1] Printing statistics from instruction bne:
     COUNT : 266
[ArchC 2.1] Printing statistics from instruction blez:
     COUNT : 2
[ArchC 2.1] Printing statistics from instruction bgtz:
     COUNT : 3
[ArchC 2.1] Printing statistics from instruction bltz:
     COUNT : 3
[ArchC 2.1] Printing statistics from instruction bgez:
     COUNT : 3
[ArchC 2.1] Printing statistics from instruction bltzal:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction bgezal:
     COUNT : 0

Total = 691 instruções
```

Outras:
```
[ArchC 2.1] Printing statistics from instruction addi:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction addiu:
     COUNT : 481
[ArchC 2.1] Printing statistics from instruction slti:
     COUNT : 4
[ArchC 2.1] Printing statistics from instruction sltiu:
     COUNT : 42
[ArchC 2.1] Printing statistics from instruction andi:
     COUNT : 115
[ArchC 2.1] Printing statistics from instruction ori:
     COUNT : 18
[ArchC 2.1] Printing statistics from instruction xori:
     COUNT : 1
[ArchC 2.1] Printing statistics from instruction lui:
     COUNT : 98
[ArchC 2.1] Printing statistics from instruction add:
     COUNT : 1
[ArchC 2.1] Printing statistics from instruction addu:
     COUNT : 432
[ArchC 2.1] Printing statistics from instruction sub:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction subu:
     COUNT : 71
[ArchC 2.1] Printing statistics from instruction slt:
     COUNT : 7
[ArchC 2.1] Printing statistics from instruction sltu:
     COUNT : 90
[ArchC 2.1] Printing statistics from instruction instr_and:
     COUNT : 19
[ArchC 2.1] Printing statistics from instruction instr_or:
     COUNT : 77
[ArchC 2.1] Printing statistics from instruction instr_xor:
     COUNT : 10
[ArchC 2.1] Printing statistics from instruction instr_nor:
     COUNT : 5
[ArchC 2.1] Printing statistics from instruction nop:
     COUNT : 1128
[ArchC 2.1] Printing statistics from instruction sll:
     COUNT : 96
[ArchC 2.1] Printing statistics from instruction srl:
     COUNT : 39
[ArchC 2.1] Printing statistics from instruction sra:
     COUNT : 46
[ArchC 2.1] Printing statistics from instruction sllv:
     COUNT : 55
[ArchC 2.1] Printing statistics from instruction srlv:
     COUNT : 45
[ArchC 2.1] Printing statistics from instruction srav:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction mult:
     COUNT : 36
[ArchC 2.1] Printing statistics from instruction multu:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction div:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction divu:
     COUNT : 72
[ArchC 2.1] Printing statistics from instruction mfhi:
     COUNT : 36
[ArchC 2.1] Printing statistics from instruction mthi:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction mflo:
     COUNT : 72
[ArchC 2.1] Printing statistics from instruction mtlo:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction sys_call:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction instr_break:
     COUNT : 0

Total = 3096 instruções
```

Aqui podemos ver facilmente o que foi dito no tópico anterior, sobre a diferença da utilização das intruções `addu` e `add`:

```
[ArchC 2.1] Printing statistics from instruction add:
     COUNT : 1
[ArchC 2.1] Printing statistics from instruction addu:
     COUNT : 432
```

Assim, tendo também os valores de `addu`, temos íncides de utilização da intrução de adição mais condizentes com o esperado. Inclusive, se fizermos uma pequena alteração em `hello.c`:

```
int a = 10;
int b = 11;
int c = a + b;
```
Agora podemos ver um incremento de um na quantidade de chamadas a `addu`, o que não podia ser visto em `add`, pois não era esta a função de adição utilizada.

```
[ArchC 2.1] Printing statistics from instruction addu:
     COUNT : 433
```

#### Contagem de CPI

Queremos aqui obter o valor de ciclos por instrução do programa `hello.c`, dado por `CPI = ciclos/instruções`.

Para calcular os ciclos, utilizaremos a tabela de CPIs médios dada na página do exercício:

|        Categoria       | CPI Médio |
|:----------------------:|:---------:|
|    Acesso à memória    |     10    |
| Controle (branch/jump) |     3     |
|         Outras         |     1     |

Assim, temos:
```
AM: 1565 * 10 = 15650
CO: 691 * 3 = 2073
OU: 3096 * 1 = 3096

TOTAL = 20819 ciclos

CPI = 20819/5352 = 3.88 ciclos/instrução
```

### Outros programas

Meu RA é 156175, baseado na tabela oferecida na descrição do exercício, farei a execução dos programas `qsort` (small), `fft` (small) e `rijndael coder` (big). Para cada programa será calculado a quantidade de ciclos para execução e seu CPI.

#### qsort (small)

Número total de instruções:
```
[ArchC 2.1] Printing GLOBAL statistics from processor module mips:
     INSTRUCTIONS : 39454159
     SYSCALLS : 10090
```

Instruções de acesso à memória:
```
[ArchC 2.1] Printing statistics from instruction lb:
     COUNT : 166956
[ArchC 2.1] Printing statistics from instruction lbu:
     COUNT : 606791
[ArchC 2.1] Printing statistics from instruction lh:
     COUNT : 100317
[ArchC 2.1] Printing statistics from instruction lhu:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction lw:
     COUNT : 8793541
[ArchC 2.1] Printing statistics from instruction lwl:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction lwr:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction sb:
     COUNT : 126944
[ArchC 2.1] Printing statistics from instruction sh:
     COUNT : 10032
[ArchC 2.1] Printing statistics from instruction sw:
     COUNT : 5993046
[ArchC 2.1] Printing statistics from instruction swl:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction swr:
     COUNT : 0

Total = 15797627 instruções
```

Instrucões de controle (branch/jump):
```
[ArchC 2.1] Printing statistics from instruction j:
     COUNT : 400888
[ArchC 2.1] Printing statistics from instruction jal:
     COUNT : 251143
[ArchC 2.1] Printing statistics from instruction jr:
     COUNT : 357034
[ArchC 2.1] Printing statistics from instruction jalr:
     COUNT : 85889
[ArchC 2.1] Printing statistics from instruction beq:
     COUNT : 547096
[ArchC 2.1] Printing statistics from instruction bne:
     COUNT : 953508
[ArchC 2.1] Printing statistics from instruction blez:
     COUNT : 47469
[ArchC 2.1] Printing statistics from instruction bgtz:
     COUNT : 1043009
[ArchC 2.1] Printing statistics from instruction bltz:
     COUNT : 76176
[ArchC 2.1] Printing statistics from instruction bgez:
     COUNT : 38155
[ArchC 2.1] Printing statistics from instruction bltzal:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction bgezal:
     COUNT : 0

Total = 3800367 instruções
```

Outras:
```
[[ArchC 2.1] Printing statistics from instruction addi:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction addiu:
     COUNT : 4804274
[ArchC 2.1] Printing statistics from instruction slti:
     COUNT : 23958
[ArchC 2.1] Printing statistics from instruction sltiu:
     COUNT : 80418
[ArchC 2.1] Printing statistics from instruction andi:
     COUNT : 434289
[ArchC 2.1] Printing statistics from instruction ori:
     COUNT : 14251
[ArchC 2.1] Printing statistics from instruction xori:
     COUNT : 347
[ArchC 2.1] Printing statistics from instruction lui:
     COUNT : 54343
[ArchC 2.1] Printing statistics from instruction add:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction addu:
     COUNT : 3456821
[ArchC 2.1] Printing statistics from instruction sub:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction subu:
     COUNT : 291084
[ArchC 2.1] Printing statistics from instruction slt:
     COUNT : 40354
[ArchC 2.1] Printing statistics from instruction sltu:
     COUNT : 235072
[ArchC 2.1] Printing statistics from instruction instr_and:
     COUNT : 24255
[ArchC 2.1] Printing statistics from instruction instr_or:
     COUNT : 88104
[ArchC 2.1] Printing statistics from instruction instr_xor:
     COUNT : 14232
[ArchC 2.1] Printing statistics from instruction instr_nor:
     COUNT : 7116
[ArchC 2.1] Printing statistics from instruction nop:
     COUNT : 10118751
[ArchC 2.1] Printing statistics from instruction sll:
     COUNT : 86139
[ArchC 2.1] Printing statistics from instruction srl:
     COUNT : 24513
[ArchC 2.1] Printing statistics from instruction sra:
     COUNT : 53496
[ArchC 2.1] Printing statistics from instruction sllv:
     COUNT : 33
[ArchC 2.1] Printing statistics from instruction srlv:
     COUNT : 25
[ArchC 2.1] Printing statistics from instruction srav:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction mult:
     COUNT : 1589
[ArchC 2.1] Printing statistics from instruction multu:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction div:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction divu:
     COUNT : 384
[ArchC 2.1] Printing statistics from instruction mfhi:
     COUNT : 364
[ArchC 2.1] Printing statistics from instruction mthi:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction mflo:
     COUNT : 1953
[ArchC 2.1] Printing statistics from instruction mtlo:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction sys_call:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction instr_break:
     COUNT : 0

Total = 19856165 instruções
```

Ciclos e CPI de qsort (small):
```
AM: 15797627 * 10 = 157976270
CO: 3800367 * 3 = 11401101
OU: 19856165 * 1 = 19856165

TOTAL = 189233536 ciclos

CPI = 189233536/39454159 = 4.79 ciclos/instrução
```

#### fft 1 (small)

Número total de instruções:
```
[ArchC 2.1] Printing GLOBAL statistics from processor module mips:
     INSTRUCTIONS : 539993292
     SYSCALLS : 139
```

Instruções de acesso à memória:
```
[ArchC 2.1] Printing statistics from instruction lb:
     COUNT : 245061
[ArchC 2.1] Printing statistics from instruction lbu:
     COUNT : 378308
[ArchC 2.1] Printing statistics from instruction lh:
     COUNT : 82553
[ArchC 2.1] Printing statistics from instruction lhu:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction lw:
     COUNT : 67625768
[ArchC 2.1] Printing statistics from instruction lwl:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction lwr:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction sb:
     COUNT : 287876
[ArchC 2.1] Printing statistics from instruction sh:
     COUNT : 137
[ArchC 2.1] Printing statistics from instruction sw:
     COUNT : 61245632
[ArchC 2.1] Printing statistics from instruction swl:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction swr:
     COUNT : 0

Total = 129865335 instruções
```

Instrucões de controle (branch/jump):
```
[ArchC 2.1] Printing statistics from instruction j:
     COUNT : 4883721
[ArchC 2.1] Printing statistics from instruction jal:
     COUNT : 9702669
[ArchC 2.1] Printing statistics from instruction jr:
     COUNT : 9719174
[ArchC 2.1] Printing statistics from instruction jalr:
     COUNT : 123
[ArchC 2.1] Printing statistics from instruction beq:
     COUNT : 31935763
[ArchC 2.1] Printing statistics from instruction bne:
     COUNT : 23350312
[ArchC 2.1] Printing statistics from instruction blez:
     COUNT : 982700
[ArchC 2.1] Printing statistics from instruction bgtz:
     COUNT : 41301
[ArchC 2.1] Printing statistics from instruction bltz:
     COUNT : 1236757
[ArchC 2.1] Printing statistics from instruction bgez:
     COUNT : 2893426
[ArchC 2.1] Printing statistics from instruction bltzal:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction bgezal:
     COUNT : 0

Total = 84745946 instruções
```

Outras:
```
[ArchC 2.1] Printing statistics from instruction addi:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction addiu:
     COUNT : 69447241
[ArchC 2.1] Printing statistics from instruction slti:
     COUNT : 4895001
[ArchC 2.1] Printing statistics from instruction sltiu:
     COUNT : 8301117
[ArchC 2.1] Printing statistics from instruction andi:
     COUNT : 9642567
[ArchC 2.1] Printing statistics from instruction ori:
     COUNT : 4707230
[ArchC 2.1] Printing statistics from instruction xori:
     COUNT : 442894
[ArchC 2.1] Printing statistics from instruction lui:
     COUNT : 13637503
[ArchC 2.1] Printing statistics from instruction add:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction addu:
     COUNT : 49742530
[ArchC 2.1] Printing statistics from instruction sub:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction subu:
     COUNT : 4608226
[ArchC 2.1] Printing statistics from instruction slt:
     COUNT : 624649
[ArchC 2.1] Printing statistics from instruction sltu:
     COUNT : 22261251
[ArchC 2.1] Printing statistics from instruction instr_and:
     COUNT : 4887747
[ArchC 2.1] Printing statistics from instruction instr_or:
     COUNT : 25315282
[ArchC 2.1] Printing statistics from instruction instr_xor:
     COUNT : 741575
[ArchC 2.1] Printing statistics from instruction instr_nor:
     COUNT : 792524
[ArchC 2.1] Printing statistics from instruction nop:
     COUNT : 35893766
[ArchC 2.1] Printing statistics from instruction sll:
     COUNT : 34395700
[ArchC 2.1] Printing statistics from instruction srl:
     COUNT : 24332764
[ArchC 2.1] Printing statistics from instruction sra:
     COUNT : 174653
[ArchC 2.1] Printing statistics from instruction sllv:
     COUNT : 1093417
[ArchC 2.1] Printing statistics from instruction srlv:
     COUNT : 1231186
[ArchC 2.1] Printing statistics from instruction srav:
     COUNT : 3714
[ArchC 2.1] Printing statistics from instruction mult:
     COUNT : 80806
[ArchC 2.1] Printing statistics from instruction multu:
     COUNT : 2660964
[ArchC 2.1] Printing statistics from instruction div:
     COUNT : 3722
[ArchC 2.1] Printing statistics from instruction divu:
     COUNT : 12478
[ArchC 2.1] Printing statistics from instruction mfhi:
     COUNT : 2677164
[ArchC 2.1] Printing statistics from instruction mthi:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction mflo:
     COUNT : 2774340
[ArchC 2.1] Printing statistics from instruction mtlo:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction sys_call:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction instr_break:
     COUNT : 0

Total = 325382011 instruções
```

Ciclos e CPI de fft 1 (small):
```
AM: 129865335 * 10 = 1298653350
CO: 84745946 * 3 = 254237838
OU: 325382011 * 1 = 325382011

TOTAL = 1878273199 ciclos

CPI = 1878273199/539993292 = 3.47 ciclos/instrução
```

#### fft 2 (small)

Número total de instruções:
```
[ArchC 2.1] Printing GLOBAL statistics from processor module mips:
     INSTRUCTIONS : 1293062436
     SYSCALLS : 195

```

Instruções de acesso à memória:
```
[ArchC 2.1] Printing statistics from instruction lb:
     COUNT : 428978
[ArchC 2.1] Printing statistics from instruction lbu:
     COUNT : 839307
[ArchC 2.1] Printing statistics from instruction lh:
     COUNT : 164753
[ArchC 2.1] Printing statistics from instruction lhu:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction lw:
     COUNT : 162521756
[ArchC 2.1] Printing statistics from instruction lwl:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction lwr:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction sb:
     COUNT : 427701
[ArchC 2.1] Printing statistics from instruction sh:
     COUNT : 193
[ArchC 2.1] Printing statistics from instruction sw:
     COUNT : 147181393
[ArchC 2.1] Printing statistics from instruction swl:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction swr:
     COUNT : 0

Total = 311564081 instruções
```

Instrucões de controle (branch/jump):
```
[ArchC 2.1] Printing statistics from instruction j:
     COUNT : 11764355
[ArchC 2.1] Printing statistics from instruction jal:
     COUNT : 23690738
[ArchC 2.1] Printing statistics from instruction jr:
     COUNT : 23723683
[ArchC 2.1] Printing statistics from instruction jalr:
     COUNT : 179
[ArchC 2.1] Printing statistics from instruction beq:
     COUNT : 76941190
[ArchC 2.1] Printing statistics from instruction bne:
     COUNT : 55857811
[ArchC 2.1] Printing statistics from instruction blez:
     COUNT : 2320558
[ArchC 2.1] Printing statistics from instruction bgtz:
     COUNT : 142340
[ArchC 2.1] Printing statistics from instruction bltz:
     COUNT : 2893182
[ArchC 2.1] Printing statistics from instruction bgez:
     COUNT : 7289947
[ArchC 2.1] Printing statistics from instruction bltzal:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction bgezal:
     COUNT : 0

Total = 204623983 instruções
```

Outras:
```
[ArchC 2.1] Printing statistics from instruction addi:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction addiu:
     COUNT : 166788171
[ArchC 2.1] Printing statistics from instruction slti:
     COUNT : 11844226
[ArchC 2.1] Printing statistics from instruction sltiu:
     COUNT : 19522316
[ArchC 2.1] Printing statistics from instruction andi:
     COUNT : 23102258
[ArchC 2.1] Printing statistics from instruction ori:
     COUNT : 11198235
[ArchC 2.1] Printing statistics from instruction xori:
     COUNT : 858392
[ArchC 2.1] Printing statistics from instruction lui:
     COUNT : 33308606
[ArchC 2.1] Printing statistics from instruction add:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction addu:
     COUNT : 117696123
[ArchC 2.1] Printing statistics from instruction sub:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction subu:
     COUNT : 9605497
[ArchC 2.1] Printing statistics from instruction slt:
     COUNT : 1974086
[ArchC 2.1] Printing statistics from instruction sltu:
     COUNT : 51905118
[ArchC 2.1] Printing statistics from instruction instr_and:
     COUNT : 11998500
[ArchC 2.1] Printing statistics from instruction instr_or:
     COUNT : 60618313
[ArchC 2.1] Printing statistics from instruction instr_xor:
     COUNT : 1841212
[ArchC 2.1] Printing statistics from instruction instr_nor:
     COUNT : 1917725
[ArchC 2.1] Printing statistics from instruction nop:
     COUNT : 84282366
[ArchC 2.1] Printing statistics from instruction sll:
     COUNT : 83226707
[ArchC 2.1] Printing statistics from instruction srl:
     COUNT : 58837345
[ArchC 2.1] Printing statistics from instruction sra:
     COUNT : 237989
[ArchC 2.1] Printing statistics from instruction sllv:
     COUNT : 2658256
[ArchC 2.1] Printing statistics from instruction srlv:
     COUNT : 2971959
[ArchC 2.1] Printing statistics from instruction srav:
     COUNT : 17760
[ArchC 2.1] Printing statistics from instruction mult:
     COUNT : 65587
[ArchC 2.1] Printing statistics from instruction multu:
     COUNT : 6748656
[ArchC 2.1] Printing statistics from instruction div:
     COUNT : 17768
[ArchC 2.1] Printing statistics from instruction divu:
     COUNT : 4
[ArchC 2.1] Printing statistics from instruction mfhi:
     COUNT : 6766428
[ArchC 2.1] Printing statistics from instruction mthi:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction mflo:
     COUNT : 6864769
[ArchC 2.1] Printing statistics from instruction mtlo:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction sys_call:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction instr_break:
     COUNT : 0

Total = 776874372 instruções
```

Ciclos e CPI de fft 2 (small):
```
AM: 311564081 * 10 = 3115640810
CO: 204623983 * 3 = 613871949
OU: 776874372 * 1 = 776874372

TOTAL = 4506387131 ciclos

CPI = 4506387131/1293062436 = 3.48 ciclos/instrução
```

#### rijndael coder enc (large)

Número total de instruções:
```
[ArchC 2.1] Printing GLOBAL statistics from processor module mips:
     INSTRUCTIONS : 453556705
     SYSCALLS : 6399
```

Instruções de acesso à memória:
```
[ArchC 2.1] Printing statistics from instruction lb:
     COUNT : 12765
[ArchC 2.1] Printing statistics from instruction lbu:
     COUNT : 6698162
[ArchC 2.1] Printing statistics from instruction lh:
     COUNT : 1455774
[ArchC 2.1] Printing statistics from instruction lhu:
     COUNT : 202973
[ArchC 2.1] Printing statistics from instruction lw:
     COUNT : 90892519
[ArchC 2.1] Printing statistics from instruction lwl:
     COUNT : 808716
[ArchC 2.1] Printing statistics from instruction lwr:
     COUNT : 808716
[ArchC 2.1] Printing statistics from instruction sb:
     COUNT : 3260361
[ArchC 2.1] Printing statistics from instruction sh:
     COUNT : 3219
[ArchC 2.1] Printing statistics from instruction sw:
     COUNT : 23612702
[ArchC 2.1] Printing statistics from instruction swl:
     COUNT : 808716
[ArchC 2.1] Printing statistics from instruction swr:
     COUNT : 808716

Total = 129373339 instruções
```

Instrucões de controle (branch/jump):
```
[ArchC 2.1] Printing statistics from instruction j:
     COUNT : 2461164
[ArchC 2.1] Printing statistics from instruction jal:
     COUNT : 1652478
[ArchC 2.1] Printing statistics from instruction jr:
     COUNT : 1658829
[ArchC 2.1] Printing statistics from instruction jalr:
     COUNT : 6351
[ArchC 2.1] Printing statistics from instruction beq:
     COUNT : 3691996
[ArchC 2.1] Printing statistics from instruction bne:
     COUNT : 4719489
[ArchC 2.1] Printing statistics from instruction blez:
     COUNT : 1
[ArchC 2.1] Printing statistics from instruction bgtz:
     COUNT : 231524
[ArchC 2.1] Printing statistics from instruction bltz:
     COUNT : 3179
[ArchC 2.1] Printing statistics from instruction bgez:
     COUNT : 615283
[ArchC 2.1] Printing statistics from instruction bltzal:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction bgezal:
     COUNT : 0

Total = 15040294 instruções
```

Outras:
```
[ArchC 2.1] Printing statistics from instruction addi:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction addiu:
     COUNT : 45774890
[ArchC 2.1] Printing statistics from instruction slti:
     COUNT : 3
[ArchC 2.1] Printing statistics from instruction sltiu:
     COUNT : 815178
[ArchC 2.1] Printing statistics from instruction andi:
     COUNT : 37800990
[ArchC 2.1] Printing statistics from instruction ori:
     COUNT : 206170
[ArchC 2.1] Printing statistics from instruction xori:
     COUNT : 2
[ArchC 2.1] Printing statistics from instruction lui:
     COUNT : 612185
[ArchC 2.1] Printing statistics from instruction add:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction addu:
     COUNT : 56217936
[ArchC 2.1] Printing statistics from instruction sub:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction subu:
     COUNT : 1030767
[ArchC 2.1] Printing statistics from instruction slt:
     COUNT : 2
[ArchC 2.1] Printing statistics from instruction sltu:
     COUNT : 1227383
[ArchC 2.1] Printing statistics from instruction instr_and:
     COUNT : 209345
[ArchC 2.1] Printing statistics from instruction instr_or:
     COUNT : 409123
[ArchC 2.1] Printing statistics from instruction instr_xor:
     COUNT : 49525505
[ArchC 2.1] Printing statistics from instruction instr_nor:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction nop:
     COUNT : 34723198
[ArchC 2.1] Printing statistics from instruction sll:
     COUNT : 45469459
[ArchC 2.1] Printing statistics from instruction srl:
     COUNT : 34099525
[ArchC 2.1] Printing statistics from instruction sra:
     COUNT : 209511
[ArchC 2.1] Printing statistics from instruction sllv:
     COUNT : 3
[ArchC 2.1] Printing statistics from instruction srlv:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction srav:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction mult:
     COUNT : 405947
[ArchC 2.1] Printing statistics from instruction multu:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction div:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction divu:
     COUNT : 1
[ArchC 2.1] Printing statistics from instruction mfhi:
     COUNT : 1
[ArchC 2.1] Printing statistics from instruction mthi:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction mflo:
     COUNT : 405948
[ArchC 2.1] Printing statistics from instruction mtlo:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction sys_call:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction instr_break:
     COUNT : 0

Total = 309143072 instruções
```

Ciclos e CPI de rijndael enc (large):
```
AM: 129373339 * 10 = 1293733390
CO: 15040294 * 3 = 45120882
OU: 309143072 * 1 = 309143072

TOTAL = 1647997344 ciclos

CPI = 1647997344/453556705 = 3.63 ciclos/instrução
```

#### rijndael coder dec (large)

Número total de instruções:
```
[ArchC 2.1] Printing GLOBAL statistics from processor module mips:
     INSTRUCTIONS : 483862588
     SYSCALLS : 6381
```

Instruções de acesso à memória:
```
[ArchC 2.1] Printing statistics from instruction lb:
     COUNT : 3247629
[ArchC 2.1] Printing statistics from instruction lbu:
     COUNT : 6698145
[ArchC 2.1] Printing statistics from instruction lh:
     COUNT : 1455748
[ArchC 2.1] Printing statistics from instruction lhu:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction lw:
     COUNT : 95535477
[ArchC 2.1] Printing statistics from instruction lwl:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction lwr:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction sb:
     COUNT : 6495177
[ArchC 2.1] Printing statistics from instruction sh:
     COUNT : 3208
[ArchC 2.1] Printing statistics from instruction sw:
     COUNT : 29276716
[ArchC 2.1] Printing statistics from instruction swl:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction swr:
     COUNT : 0

Total = 142712100 instruções
```

Instrucões de controle (branch/jump):
```
[ArchC 2.1] Printing statistics from instruction j:
     COUNT : 2467494
[ArchC 2.1] Printing statistics from instruction jal:
     COUNT : 1652445
[ArchC 2.1] Printing statistics from instruction jr:
     COUNT : 1658792
[ArchC 2.1] Printing statistics from instruction jalr:
     COUNT : 6347
[ArchC 2.1] Printing statistics from instruction beq:
     COUNT : 3083016
[ArchC 2.1] Printing statistics from instruction bne:
     COUNT : 7361228
[ArchC 2.1] Printing statistics from instruction blez:
     COUNT : 1
[ArchC 2.1] Printing statistics from instruction bgtz:
     COUNT : 218837
[ArchC 2.1] Printing statistics from instruction bltz:
     COUNT : 3178
[ArchC 2.1] Printing statistics from instruction bgez:
     COUNT : 202995
[ArchC 2.1] Printing statistics from instruction bltzal:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction bgezal:
     COUNT : 0


Total = 16654333 instruções
```

Outras:
```
[ArchC 2.1] Printing statistics from instruction addi:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction addiu:
     COUNT : 52444416
[ArchC 2.1] Printing statistics from instruction slti:
     COUNT : 3
[ArchC 2.1] Printing statistics from instruction sltiu:
     COUNT : 206253
[ArchC 2.1] Printing statistics from instruction andi:
     COUNT : 38007240
[ArchC 2.1] Printing statistics from instruction ori:
     COUNT : 20
[ArchC 2.1] Printing statistics from instruction xori:
     COUNT : 3
[ArchC 2.1] Printing statistics from instruction lui:
     COUNT : 406039
[ArchC 2.1] Printing statistics from instruction add:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction addu:
     COUNT : 56221132
[ArchC 2.1] Printing statistics from instruction sub:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction subu:
     COUNT : 1030738
[ArchC 2.1] Printing statistics from instruction slt:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction sltu:
     COUNT : 1024420
[ArchC 2.1] Printing statistics from instruction instr_and:
     COUNT : 3197
[ArchC 2.1] Printing statistics from instruction instr_or:
     COUNT : 405950
[ArchC 2.1] Printing statistics from instruction instr_xor:
     COUNT : 49525661
[ArchC 2.1] Printing statistics from instruction instr_nor:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction nop:
     COUNT : 44424303
[ArchC 2.1] Printing statistics from instruction sll:
     COUNT : 45469582
[ArchC 2.1] Printing statistics from instruction srl:
     COUNT : 34511965
[ArchC 2.1] Printing statistics from instruction sra:
     COUNT : 3331
[ArchC 2.1] Printing statistics from instruction sllv:
     COUNT : 3
[ArchC 2.1] Printing statistics from instruction srlv:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction srav:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction mult:
     COUNT : 405948
[ArchC 2.1] Printing statistics from instruction multu:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction div:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction divu:
     COUNT : 1
[ArchC 2.1] Printing statistics from instruction mfhi:
     COUNT : 1
[ArchC 2.1] Printing statistics from instruction mthi:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction mflo:
     COUNT : 405949
[ArchC 2.1] Printing statistics from instruction mtlo:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction sys_call:
     COUNT : 0
[ArchC 2.1] Printing statistics from instruction instr_break:
     COUNT : 0

Total = 324496155 instruções
```

Ciclos e CPI de rijndael dec (large):
```
AM: 142712100 * 10 = 1427121000
CO: 16654333 * 3 = 49962999
OU: 324496155 * 1 = 324496155

TOTAL = 1801580154 ciclos

CPI = 1801580154/483862588 = 3.72 ciclos/instrução
```
