- Henrique Noronha Facioli - 157986
- Lauro Cruz e Souza - 156175
- Thiago Silva Farias - 148077

# Roteiro do trabalho 2

## Configurações
### - Tamanho do pipeline
- 5 estágios: escalar e superescalar
- 7 estágios: escalar
- 13 estágios: escalar

O simulador do MIPS não oferece uma implementação de pipeline. Sendo assim, teremos que modificar os códigos para contar os ciclos.

### - Hazard de dados e controle
Teremos que alterar o código do simulador do MIPS para obter os hazards, pois será necessário manter um contador de hazards no código.
No caso do processador escalar, podemos encontrar hazards de dados do tipo RAW (Read After Write), adicionando um ou dois ciclos de stall a mais, dependendo das instruções que causaram a dependência.
No processador superescalar, podem ocorrer hazards de dados do tipo WAR (Write After Read) e WAW (Write After Write). Nesses caso será contabilizado um ciclo adicional.
Hazards de controle ocorrem quando não tem branch predictor, ou quando este erra na predição. O número de ciclos adicionados nesse caso dependerá dos tipos de branch e de branch predictor.

### - Branch predictor (3 configurações distintas)
- Sem predictor (sempre atrasa pipeline ao encontrar um branch)
- Always taken
- Two-bit prediction

### - Cache
- Sizes:
    - L1: 16KB, 32KB, 64KB
    - L2: 128KB, 256KB, 512KB
- Block sizes:
    - L1: 16, 32
    - L2: 64, 128
- Associatividade: 1, 2

## Programas a ser utilizados
- `patricia`
- `basicmath`
- `fft`

Escolhemos esses três pois foram os que tiveram a execução mais demorada em nossos exercícios (3).

## Eventos a serem medidos
- Ciclos
- Branch misses
- Cache L1/L2 misses
- Tempo de execução
- CPI
- Hazard de dados e controle

## Configurações a serem testadas

### Configurações de Cache

| Configuração | L1 Cache Size | L2 Cache Size | L1 Block Size | L2 Block Size | Associativity |
|--------------|---------------|---------------|---------------|---------------|---------------|
| 1            | 64 KB         | 256 KB        | 32 B          | 128 B         | 2             |
| 2            | 16 KB         | 256 KB        | 32 B          | 128 B         | 2             |
| 3            | 32 KB         | 256 KB        | 32 B          | 128 B         | 2             |
| 4            | 64 KB         | 128 KB        | 32 B          | 128 B         | 2             |
| 5            | 64 KB         | 512 KB        | 32 B          | 128 B         | 2             |
| 6            | 64 KB         | 256 KB        | 16 B          | 128 B         | 2             |
| 7            | 64 KB         | 256 KB        | 32 B          | 64 B          | 2             |
| 8            | 64 KB         | 256 KB        | 32 B          | 128 B         | 1             |

### Cenários

| #  | Pipeline                   | Configuração de Cache | Branch Predictor |
|----|----------------------------|-----------------------|------------------|
| 1  | Escalar de 5 estágios      | 1                     | Sem              |
| 2  | Superescalar de 5 estágios | 1                     | Sem              |
| 3  | Escalar de 7 estágios      | 1                     | Sem              |
| 4  | Escalar de 13 estágios     | 1                     | Sem              |
| 5  | Escalar de 5 estágios      | 1                     | Sem              |
| 6  | Escalar de 5 estágios      | 2                     | Sem              |
| 7  | Escalar de 5 estágios      | 3                     | Sem              |
| 8  | Escalar de 5 estágios      | 4                     | Sem              |
| 9  | Escalar de 5 estágios      | 5                     | Sem              |
| 10 | Escalar de 5 estágios      | 6                     | Sem              |
| 11 | Escalar de 5 estágios      | 7                     | Sem              |
| 12 | Escalar de 5 estágios      | 8                     | Sem              |
| 13 | Escalar de 5 estágios      | 1                     | Always Taken     |
| 14 | Escalar de 5 estágios      | 1                     | Two-Bit          |
| 15 | Superescalar de 5 estágios | 1                     | Always Taken     |
| 16 | Superescalar de 5 estágios | 1                     | Two-Bit          |
