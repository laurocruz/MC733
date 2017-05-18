# Projeto 2: Desempenho do Processador

## Grupo 3
- Henrique Noronha Facioli - 157986
- Lauro Cruz e Souza - 156175
- Thiago Silva Farias - 148077


## Introdução
O desempenho de um processador vai além de estatísticas como o seu número de núcleos ou a frequência de seu *clock* - é fundamental ter uma arquitetura bem planejada, de forma a lidar bem com as sequências de instruções mais comuns nos diversos programas com os quais ele deve lidar.

Tendo isso em mente, realizamos a simulação do processamento de quatro benchmarks diferentes, variando as  configurações de forma a criar diversos cenários da arquitetura do processador. Com isso, calculamos diferentes eventos que nos permitiram analisar a eficiência de cada cenário, assim como a influência de cada uma dessas configurações na performance do processador.

Caso o leitor tenha interesse, criamos um breve tutorial de como replicar a execução do experimento em [HOWTO.md](mips_files/HOWTO.md).

## Metodologia

### Benchmarks utilizados
Foram analisados os benchmarks vistos pelos integrantes do grupo na atividade 3. Foram escolhidos aqueles que tiveram o maior tempo de execução.
- `patricia`
- `basicmath`
- `fft_encode`
- `fft_decode`

### Configurações
Foram variadas diversas características da arquitetura do processador, de forma a se analisar o impacto de cada uma dessas mudanças individualmente. Para isso, foram feitas mudanças no código do `mips_isa.cpp`, de forma a contabilizar os eventos relevantes para os cenários testados.

A seguir, apresentamos as características que foram variadas, as configurações utilizadas, bem como uma breve explicação de como o código do `mips_isa.cpp` foi alterado para que os eventos relevantes fossem calculados.

##### Tipo de Pipeline
Em nosso experimento utilizamos dois tamanhos de pipeline, o de 5 estágios e o de 7 estágios.
No pipeline de 5 estágios, dividimos a execução de uma intrução em 5 etapas `|IF|ID|EX|MEM|WB` na tentativa de executar cada instrução em um ciclo. Com o pipeline de 5 estágios também implementamos a ideia de ser escalar ou superscalar. Nos processadores escalares existe apenas um único pipeline e as instrução são executadas sequencialmente enquanto que no superescalar de dois níveis existem dois pipelines independentes que executam instruções concorrentemente.
Já no pipeline de 7 estágios, a execução de uma instrução é dividido em 7 etapas `| IT | IF | ID | EX | MT | MM | WB |` em que a 

- Escalar.
- Superescalar.

##### Tamanho do Pipeline
- 5 estágios.
- 7 estágios.

##### Configurações de Cache
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

##### Branch Predictor
- Sem predictor (sempre atrasa pipeline ao encontrar um branch).
- Always not taken.
- Two-bit prediction.

### Cenários Analisados
A seguir apresentamos uma tabela que identifica as diferentes combinações das configurações que foram medidas. Estas combinações foram escolhidas de forma a facilitar a análise da influência das mudanças em cada configuração individualmente, além de nos fornecer uma visão geral das vantagens e desvantagens de cada cenário.

| Cenário  | Pipeline                   | Configuração de Cache | Branch Predictor |
|----|----------------------------|-----------------------|------------------|
| 1  | Escalar de 5 estágios      | 1                     | Sem              |
| 2  | Superescalar de 5 estágios | 1                     | Sem              |
| 3  | Escalar de 7 estágios      | 1                     | Sem              |
| 4  | Escalar de 5 estágios      | 2                     | Sem              |
| 5  | Escalar de 5 estágios      | 3                     | Sem              |
| 6  | Escalar de 5 estágios      | 4                     | Sem              |
| 7  | Escalar de 5 estágios      | 5                     | Sem              |
| 8  | Escalar de 5 estágios      | 6                     | Sem              |
| 9  | Escalar de 5 estágios      | 7                     | Sem              |
| 10 | Escalar de 5 estágios      | 8                     | Sem              |
| 11 | Escalar de 5 estágios      | 1                     | Always Not Taken |
| 12 | Escalar de 5 estágios      | 1                     | Two-Bit          |
| 13 | Superescalar de 5 estágios | 1                     | Always Not Taken |
| 14 | Superescalar de 5 estágios | 1                     | Two-Bit          |

## Resultados

### Eventos medidos
De forma a analisar os diferentes cenários simulados, foram determinados os seguintes eventos para serem utilizados como base:
- CPI.
- Número de Ciclos.
- Tempo de execução.
- Número de Instruções.
- Número de Intruções de cada tipo (R, I, J).
- Quantidade de Hazards de Dados.
- Número de Stalls devido a Hazards de Dados.
- Quantidade de Hazards de Controle.
- Número de Stalls devido a Hazards de Controle.
- Número de Stalls devido a Jumps.
- Número de Cache Misses na L1.
- Porcentagem de Cache Misses na L1.
- Número de Cache Misses na L2.
- Porcentagem de Cache Misses na L2.
- Número de Stalls devido a Cache Misses.
- Total de Stalls.
- Número de Branches.
- Quantidade de Branch Predictions Corretos.
- Quantidade de Branch Predictions Incorretos.

Boa parte desses eventos foram obtidos diretamente da simulação feita, porém, alguns outros precisaram ser estimados. Abaixo, listamos esses casos, apresentamos a fórmula utilizada e uma breve explicação dos valores utilizados.

##### Total de Stalls

##### Número de Ciclos

##### CPI

##### Tempo de Execução


### Análise dos Resultados e Conclusão
