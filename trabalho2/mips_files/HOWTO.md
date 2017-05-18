# Simulação do pipeline e verificação de memória
## Como Executar
Para executar algumas variáveis do ambiente precisam ser definidar. Para definir uma variável utilize `export VARIVAVEL=valor`.
### Variaveis a serem definidas
 - `export PATH=PATH_TO_mips-newlib-elf/bin:$PATH` - [Download](http://archc.lsc.ic.unicamp.br/downloads/Tools/mips/archc_mips_toolchain_20141215_64bit.tar.bz2)
 - `export SIMULATOR="PATH_TO_MIPS/mips.x --load="` - Compilar antes - [Download](https://github.com/ArchC/mips/archive/v2.4.0.tar.gz)
 - `export DINERO_SRC=PATH_TO_DINERO` - Compilar antes - [Download](ftp://ftp.cs.wisc.edu/markhill/DineroIV/d4-7.tar.gz)
 - `export BENCH=PATH_TO_MipsMibench/`
e também carregue as env do archc com
 - `source PATH_TO_ARCHC/etc/env.sh` - [Download](https://github.com/ArchC/ArchC/archive/v2.4.1.tar.gz)

### Executando
Após fazer todos os exports corretos, é necessário adicionar os arquivos nos locais certos. Substitua o `mips_isa.cpp` e o `Makefile` da pasta `PATH_TO_MIPS` pelos respectivos arquivos deste repositório. Para rodar automaticamente os `testes patricia`, `basicmath_small` e `fft` coloque o arquivo `run_tests.sh` no mesmo repositório e execute-o.

## Arquivos
### mips_isa.cpp
O mips_isa é o arquivo que implementa como as intruções são executadas pelo archC, por isso foi modificado.
Abaixo está uma pequena descrição do que pensamos ao implementar o o pipeline e o branchprediction

```
/* ~~~ Implementação do Branch Prediction e Pipeline ~~~
 * Primeiramente, precisamos saber qual a instrução está sendo executada e seus
 * parâmetros - por isso, temos esse struct que possui qual o tipo de instrução
 * (inst) que pode ser uma instrução do formato R, I ou J
 *      R: Usada para operações aritméticas
 *         Opcode - rs - rt - rd - shift amt - functional
 *         rs ~> src | rt ~> src | rd ~> dst
 *      I: Usada para operações de load e store e jump condicional
 *         opcode - rs - rt - address
 *         rs ~> base| rt ~> dst
 *      J: Usada para jump não condicional
 *         Opcode - address
 * Algumas obs:
 *  - Inst com imediato usa instrução do tipo I
 * mais info: homepage.cs.uiowa.edu/~ghosh/1-24-06.pdf
 *
 *
 * Para facilitar, todas as instruçãoes são resolvidas no WB
 * Jumps são resolvidos no segundo estágio
 * ~~~~ Explicações dos pipelines ~~~~
 * ~~ Pipeline de 5 Estágios ~~
 * ~ Escalar
 * | IF | ID | EX | MEM | WB |
 *  Data Hazards nos seguintes casos (Sem Fowarding):
 *    - EX|MEM.rd = ID/EX.rs ou ID/EX.rt - 2 stalls
 *    - MEM|WB.rd = ID/EX.rs ou ID/EX.rt - 1 stalls
 *  Control Hazards:
 *    - Sem Branch Prediction:
 *      - O pipeline para a execução até calcular o branch - 3 stalls
 *    - Always not Taken
 *      - Se tomar o branch - 4 stalls
 *      - Se não tomar      - 0 stalls
 *    - Two-bit (não toma <-> não toma <-> toma <-> toma )
 *      - Se nao tomar e for para tomar e vice e versa - 4 stalls
 *
 * ~ Superescalar de dois níveis in-order-commit
 * | IF | ID | EX | MEM | WB |
 * | IF | ID | EX | MEM | WB |
 *  Data Hazards nos seguintes casos (Sem Fowarding):
 *    -  1EX|1MEM.rd = 1ID/1EX.rs ou 1ID/1EX.rt - 2 stalls-|
 *    -  1MEM|1WB.rd = 1ID/1EX.rs ou 1ID/1EX.rt - 1 stalls |\ Casos escalar
 *    -  2EX|2MEM.rd = 2ID/2EX.rs ou 2ID/2EX.rt - 2 stalls |/
 *    -  2MEM|2WB.rd = 2ID/2EX.rs ou 2ID/2EX.rt - 1 stalls-|
 *    Como eles devem estar alinhados (ioc) dobra os números de stalls
 *    -  1EX|1MEM.rd = 2ID|2EX.rs ou 2ID/2EX.rt - 2 stalls
 *    -  1MEM|1WB.rd = 2ID/2EX.rs ou 2ID/2EX.rt - 1 stalls
 *    -  2EX|2MEM.rd = 1ID/1EX.rs ou 1ID/1EX.rt - 2 stalls
 *    -  2MEM|2WB.rd = 1ID/1EX.rs ou 1ID/1EX.rt - 1 stalls
 *
 * ~~ Pipeline de 7 estágios ~~
 * | IT | IF | ID | EX | MT | MM | WB |
 *  Data Hazards nos seguintes casos (Sem Fowarding):
 *    - EX|MT.rd = ID|EX.rs ou ID|EX.rt - 3 stalls
 *    - MT|MM.rd = ID|EX.rs ou ID|EX.rt - 2 stalls
 *    - MM|WB.rd = ID|EX.rs ou ID|EX.rt - 1 stalls
 *  Control Hazards:
 *    - Sem Branch Prediction:
 *      - O pipeline para a execução até calcular o branch - 5 stalls
 *
 */
 ```
