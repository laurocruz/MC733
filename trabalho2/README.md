- Henrique Noronha Facioli - 157986
- Lauro Cruz e Souza - 156175
- Thiago Silva Farias - 148077

# Roteiro do trabalho 2

## Configurações
### - Tamanho do pipeline
- 5 estágios: escalar e superescalar
- 7 estágios: escalar
- 13 estágios: escalar

### - Hazard de dados e controle

### - Branch predictor (2 configurações distintas)
- Sem predictor (sempre atrasa pipeline ao encontrar um branch)
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
- `gsm enconde`
- `fft`

Escolhemos esses três pois foram os que tiveram a execução mais demorada em nossos exercícios (3).

## Eventos a serem medidos
- Ciclos
- Branch misses
- Cache L1/L2 misses
- Tempo de execução
- CPI
- Hazard de dados e controle
