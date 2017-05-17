# Projeto 2: Desempenho do Processador

## Grupo 3
- Henrique Noronha Facioli - 157986
- Lauro Cruz e Souza - 156175
- Thiago Silva Farias - 148077


## Introdução
O desempenho de um processador vai além de números como o seu número de núcleos ou a frequência de seu *clock* - é preciso ter uma arquitetura bem planejada, de forma a lidar bem com as sequências de instruções mais comuns nos diversos programas com os quais ele deve lidar.

Tendo isso em mente, realizamos o benchmark de três programas diferentes, com diferentes configurações de arquitetura do processador. Com isso, calculamos diferentes eventos que nos permitam analisar a eficiência de cada uma dessas configurações.

## Benchmarks utilizados
Foram analisados os benchmarks vistos pelos integrantes do grupo na atividade 3. Foram escolhidos aqueles que tiveram o maior tempo de execução.
- `patricia`
- `basicmath`
- `fft`

## Configurações
Foram variadas diversas características da arquitetura do processador, de forma a se analisar o impacto de cada uma dessas mudanças individualmente. Assim, foram variadas as propriedades abaixo.

### Tipo de Pipeline
- Escalar.
- Superescalar.

### Tamanho do Pipeline
- 5 estágios.
- 7 estágios.

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

### Branch Predictor
- Sem predictor (sempre atrasa pipeline ao encontrar um branch)
- Always not taken
- Two-bit prediction

## Eventos medidos
De forma a analisar os diferentes cenários simulados, foram determinados os seguintes eventos para serem utilizados como base:
- CPI.
- Número de Ciclos.
- Tempo de execução.
- Número de Instruções.
- Número de Intruções de cada tipo (R, I, J).
- Quantidade de Hazard de Dados.
- Número de Stalls devido a Hazard de Dados.
- Quantidade de Hazard de Controle.
- Número de Stalls devido a Hazard de Controle.
- Número de Stalls devido a Jumps.
- Total de Stalls.
- Número de Branches.
- Quantidade de Branch Predictions Corretos.
- Quantidade de Branch Predictions Incorretos.
- Porcentagem de Cache Misses na L1.
- Porcentagem de Cache Misses na L2.
