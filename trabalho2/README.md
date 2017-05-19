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
Foram analisados alguns dos benchmarks utilizados pelos integrantes do grupo na atividade 3. Foram escolhidos aqueles que tiveram o maior tempo de execução.
- `patricia`
- `basicmath`
- `fft_encode`
- `fft_decode`

### Configurações
Foram variadas diversas características da arquitetura do processador, de forma a se analisar o impacto de cada uma dessas mudanças individualmente. Para isso, foram feitas mudanças no código do `mips_isa.cpp`, de forma a contabilizar os eventos relevantes para os cenários testados.

A seguir, apresentamos as características que foram variadas, as configurações utilizadas, bem como uma breve explicação de como o código do `mips_isa.cpp` foi alterado para que os eventos relevantes fossem calculados.

#### Pipeline
Em nosso experimento utilizamos dois tamanhos de pipeline, o de 5 estágios e o de 7 estágios.
No pipeline de 5 estágios, dividimos a execução de uma instrução em 5 etapas `| IF | ID | EX | MEM | WB |`.
Com o pipeline de 5 estágios também implementamos a ideia de ser escalar ou superscalar. Nos processadores escalares existe apenas um único pipeline e as instruções são executadas sequencialmente, enquanto que no superescalar de dois níveis existem dois pipelines independentes que executam instruções concorrentemente (ver imagem abaixo).

![BP x T](img/Hazards/Pipeline5.small.jpg) ![BP x T](img/Hazards/Pipeline5Super.small.jpg)

No pipeline de 5 estágios ocorrem hazard de dados quando uma instrução no estágio `ID | EX` utiliza algum dado que será calculado ainda, ou seja, existe alguma instrução em `EX | MEM` ou `MEM | WB` cuja saída será sua entrada. Quando acontece isso o pipeline precisa "parar" seu andamento e isso é chamado de stall. Para facilitar a implementação, nosso pipeline não possui forwarding e os dados são retornados ao `ID` no final de `WB`, portanto podemos ter 1 ou 2 stalls dependendo de onde estiver a dependência.
Os hazards de controle acontecem quando existe alguma interrupção do pipeline por conta do funcionamento de alguma instrução como branch e jump. Para as intruções de jump podemos verificar o seu endereço no `EX` e portanto precisamos de 2 stalls para o salto. Já para as intruções de branch podemos [implementar maneiras diferentes](#branch-predictor) afim de obter melhores resultados.

![BP x T](img/Hazards/HazardDadosPipeline5.small.jpg) ![BP x T](img/Hazards/HazardControlePipeline5.small.jpg)

Já no pipeline de 7 estágios, a execução de uma instrução é dividido em 7 etapas `| IT | IF | ID | EX | MT | MM | WB |` em que a etapa `IF` do pipeline de 5 estágios foi dividida em duas etapas: a `IT` e `IF` e a `MEM` dividida em `MT` e `MM`. Assim como no pipeline de 5 estágios, consideramos que as instruções são concluídas no `WB` e, analogamente ao de 5, podemos ter 1, 2 e 3 stalls para hazard de dados. Para hazard de controle, para a instrução de jump teremos 3 stalls.

![BP x T](img/Hazards/Pipeline7.small.jpg)

#### Configurações de Cache
No trabalho foram simuladas 4 caches: L1 e L2 de dados e de instruções. Fizemos simulações para oito variações das configurações de L1 e L2 (tanto de dados quanto de instruções). Abaixo segue a lista das configurações:

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

Para realizar a simulação foi utilizada a API do DineroIV, realizando uma referência às caches de instrução sempre que uma instrução fosse chamada e uma referência às caches de dado sempre que fosse chamada uma instrução de leitura/escrita.

Obtivemos ao final a quantidade de fetches e de misses em cada cache, bem como o percentual total de misses, que são os valores que serão usados para comparar as caches.

#### Branch Predictor
Uma das causas dos Hazards de Controle são as instruções de branch. Na tentativa de minimizar o número de stalls com essas intruções existem estratégias que tentam prever a ocorrencia ou não de branches.
1. **Sem predictor**:
    Sem a implementação de algum tipo de brench prediction o pipeline gera stalls no `IF` até que o branch tenha se resolvido. Dessa maneira, independentemente do branch ocorrer ou não, teremos o `tamanho do pipeline - 1` stalls.
2. **Always not taken**:
    Nesta abordagem o pipeline sempre considera que o branch não ocorrerá. Se chegar no `WB` e ele não ocorrer, nenhum stall será adicionado, mas se ele ocorrer as instruções que estão no pipeline deverão ser descartadas e assim teremos `tamanho do pipeline - 1` stalls.
3. **Two-bit prediction**:
    Na abordagem da previsão com dois bits, dois bits são utilizados de maneira a ter uma memória das últimas execuções do branch. Assim, o two-bit prediction possui 2 estados em que cada estado possui 2 representações e dependendo do estado ele poderá funcionar como o *not taken* ou *taken*.

![BP x T](img/Hazards/2bitpred.png)

### Cenários Analisados
A seguir apresentamos uma tabela que identifica as diferentes combinações de configurações que foram medidas. Estas combinações foram escolhidas de forma a facilitar a análise da influência das mudanças em cada configuração individualmente, além de nos fornecer uma visão geral das vantagens e desvantagens de cada cenário.

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
Em seguida, apresentaremos os resultados mais relevantes encontrados pelo grupo, bem como análises referentes às diferentes características da arquitetura do processador.

Caso o leitor tenha interesse, montamos uma planilha com todos os dados coletados pela simulação em [resultados.ods](resultados.ods).


### Eventos medidos
De forma a analisar os diferentes cenários simulados, foram determinados os seguintes eventos para serem utilizados como base:
- CPI (`CPI`).
- Número de Ciclos (`NC`).
- Tempo de execução (`T`).
- Número de Instruções (`NI`).
- Número de Intruções de cada tipo (R, I, J).
- Quantidade de Hazards de Dados.
- Número de Stalls devido a Hazards de Dados (`SHD`).
- Quantidade de Hazards de Controle.
- Número de Stalls devido a Hazards de Controle (`SHC`).
- Número de Stalls devido a Jumps (`SJ`).
- Número de Cache Misses na L1 (`NCM1`).
- Porcentagem de Cache Misses na L1.
- Número de Cache Misses na L2 (`NCM2`).
- Porcentagem de Cache Misses na L2.
- Número de Stalls devido a Cache Misses.
- Total de Stalls (`TS`).
- Número de Branches.
- Quantidade de Branch Predictions Corretos.
- Quantidade de Branch Predictions Incorretos.

Boa parte desses eventos foram obtidos diretamente da simulação feita, porém, alguns outros precisaram ser estimados. Abaixo, listamos esses casos, apresentamos a fórmula utilizada e uma breve explicação da motivação por trás dessas estimativas.

#### Total de Stalls
O número total de stalls é a soma do número de stalls devido a hazards de dados com o número de stalls devido a hazards de controle, com o número de stalls devido a jumps e com o número de stalls devido a cache misses. Para este último, estimamos que cache hits não acarretam nenhum stall adicional, que um cache miss na L1 causa por volta de 15 stalls adicionais e que um cache miss na L2 causa em torno de 200 stalls adicionais. Assim, a fórmula para o número total de stalls é:

`TS = SHD + SHC + SJ + ((15 · NCM1) + (200 · NCM2))`

#### Número de Ciclos
O número de ciclos calculado para pipelines escalares foi o resultado da soma do número de instruções, com o número total de stalls, com o número de estágios do pipeline (`NEP`), subtraído de 1. A fórmula para o número de ciclos é:

`NC = NI + TS + (NEP - 1)`

#### CPI
O cálculo do CPI é, por definição, a divisão do número de ciclos pelo número de instruções:

`CPI = NC / NI`

#### Tempo de Execução
Para o cálculo do tempo de execução, primeiramente foi necessário definir a frequência (`F`) do *clock* em um processador MIPS. Após pesquisas feitas pelo grupo, definimos como razoáveis os valores de 150 MHz para um pipeline de 5 estágios e de 200 MHz para um pipeline de 7 estágios. Assim, o tempo de execução será de:

`T = NC / F`


### Análise

#### Escalar x Superescalar
Com o objetivo de analisar a influência da paralelização das instruções na eficiência do processador, foi feita uma comparação dando ênfase aos cenários 1 e 2, visto que a única diferença entre eles é a de que a primeira é escalar e a segunda, superescalar.
Como podemos ver nos gráficos abaixo, a configuração superescalar consegue um CPI menor quando comparado à escalar (lembramos que quanto menor o CPI, melhor) já que consegue executar o pipeline em paralelo (2x mais instruções), e com isso, o tempo diminui.
No entanto, é interessante notar que na configuração superescalar exitem maiores probabilidades de ocorrerem hazard de dados do que na escalar e isso pode ser visto no gráfico do número total de stalls, onde em todas as execuções os números de stalls no pipeline foram maiores que no escalar. Apesar disso, a dupla execução do pipeline ainda compensou o aumento de hazards.

![TP x CPI](img/Charts/TPxCPI.small.png) ![NEP x T](img/Charts/TPxT.small.png) ![NEP x T](img/Charts/TPxStalls.small.png)

#### Número de Estágios do Pipeline
De forma a analisar a influência do número de estágios do pipeline na eficiência do processador, foi feita uma comparação dando ênfase aos cenários 1 e 3, visto que eles diferem somente no número de estágios.

Com isso, pudemos observar que com o aumento do número de estágios, houve um aumento no número de ciclos, e consequente aumento de CPI. Porém, o tempo de execução sofreu um decréscimo considerável, devido à diminuição do tempo necessário para execução de cada estágio no pipeline de 7 estágios, fato que decorre da diferença de frequência entre os pipelines de 5 e 7 estágios (vide [tempo de execução](#tempo-de-execução)). Abaixo seguem dois gráficos que ilustram essas diferenças:

![NEP x CPI](img/Charts/NEPxCPI.small.png) ![NEP x T](img/Charts/NEPxT.small.png)

#### Cache
Para as configurações de cache, foram analisados os cenários 1, 4, 5, 6, 7, 8, 9 e 10, visto que eles variam somente a [configuração de cache](#configurações-de-cache). Assim podemos avaliar apenas a influência das configurações da cache nos resultados.

##### Cache de dados

##### Cache de instruções

#### Branch Predictor
Por fim, para analisar a influência do branch predictor na eficiência do processador, foi feita uma comparação focada nos cenários 1, 11 e 12, uma vez que eles diferem somente no tipo de branch predictor utilizado.
Em termos de tempo e CPI, o Two Bit Predictor se mostrou superior ao Always Not Taken, que, por sua vez, superou o cenário sem nenhum tipo de Branch Predictor, como mostram os gráficos a seguir:

![BP x CPI](img/Charts/BPxCPI.small.png) ![BP x T](img/Charts/BPxT.small.png)

Além disso, foram feitas comparações da eficácia do Two Bit Predictor e do Always Not Taken, através da porcentagem de branch predictions corretos (ou seja, o número de branch predictions corretos dividido pelo número total de branches). Mais uma vez, como podemos ver pelo gráfico abaixo, o Two Bit Predictor se mostrou superior.

![BP x PBC](img/Charts/BPxPBC.small.png)

### Conclusão
Finalmente, apresentamos uma tabela contendo os resultados mais relevantes obtidos neste experimento:

|                             | Cenário 1 |            |            |            | Cenário 2 |            |            |            | Cenário 3 |            |            |            | Cenário X |           |            |            | Cenário 11 |            |            |            | Cenário 12 |            |            |            |
|-----------------------------|-----------|------------|------------|------------|-----------|------------|------------|------------|-----------|------------|------------|------------|-----------|-----------|------------|------------|------------|------------|------------|------------|------------|------------|------------|------------|
|                             | patricia  | basicmath  | fft_encode | fft_decode | patricia  | basicmath  | fft_encode | fft_decode | patricia  | basicmath  | fft_encode | fft_decode | patricia  | basicmath | fft_encode | fft_decode | patricia   | basicmath  | fft_encode | fft_decode | patricia   | basicmath  | fft_encode | fft_decode |
| Ciclos                      | 768467250 | 2142071587 | 971452775  | 2444935097 | 732771133 | 1989104161 | 891505516  | 2244400562 | 843748840 | 2466534069 | 1113566094 | 2801572165 |           |           |            |            | 730571025  | 1938090031 | 854165855  | 2150448698 | 726513533  | 1907776300 | 854313522  | 2150873511 |
| CPI                         | 3.401     | 1.966      | 1.799      | 1.799      | 3.243     | 1.825      | 1.651      | 1.652      | 3.734     | 2.263      | 2.062      | 2.062      |           |           |            |            | 3.233      | 1.778      | 1.582      | 1.583      | 3.215      | 1.751      | 1.582      | 1.583      |
| Tempo (em segundos)         | 5.12      | 14.28      | 6.48       | 16.30      | 4.89      | 13.26      | 5.94       | 14.96      | 4.22      | 12.33      | 5.57       | 14.01      |           |           |            |            | 4.87       | 12.92      | 5.69       | 14.34      | 4.84       | 12.72      | 5.70       | 14.34      |
| Total de Stalls             | 542514101 | 1052306847 | 431459479  | 1086121776 | 619794556 | 1444221789 | 621508866  | 1564993899 | 617795689 | 1376769327 | 573572796  | 1442758842 |           |           |            |            | 504617876  | 848325291  | 314172559  | 791635377  | 500560384  | 818011560  | 314320226  | 792060190  |
|                             |           |            |            |            |           |            |            |            |           |            |            |            |           |           |            |            |            |            |            |            |            |            |            |            |
| Branch Predictions Corretos | -         | -          | -          | -          | -         | -          | -          | -          | -         | -          | -          | -          | -         | -         | -          | -          | 12632075   | 67993852   | 39095640   | 98162133   | 14913792   | 82017241   | 40481662   | 101721166  |
| Número de Branches          | -         | -          | -          | -          | -         | -          | -          | -          | -         | -          | -          | -          | -         | -         | -          | -          | 24202474   | 126051956  | 60440259   | 151639479  | 24202474   | 126051956  | 60440259   | 151639479  |



A partir desses dados, podemos concluir que:
 - Processadores superescalares chegam a ser 10% mais rápidos aos escalares.
 - Quanto ao número de estágios, se a frequência do clock fosse a mesma, processadores de 5 estágios seriam superiores aos de 7 estágios. Porém, levando em conta a diferença na frequência do clock gerada devido ao aumento do número de estágios, os processadores de 7 estágios apresentaram tempos até 20% melhores que os de 5 estágios.
 - FAZER CONCLUSÃO SOBRE AS CACHES.
 - Por fim, quanto aos branch predictors, o Two Bit Predictor se mostrou a melhor escolha, tanto no que diz respeito aos tempos de execução quanto à porcentagem de acertos de branches. Em relação ao Always Not Taken, ele apresentou uma taxa de acertos até 10% superior.
