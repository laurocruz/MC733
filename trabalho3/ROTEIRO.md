# Roteiro do Projeto 3: Multicore e Aceleração em Hardware

## Grupo 3
- Henrique Noronha Facioli - 157986
- Lauro Cruz e Souza - 156175
- Thiago Silva Farias - 148077


## Software a executar
O software escolhido pelo grupo para o projeto 3 foi o `FFT` (Fast Fourier Transform), que é uma ferramenta versátil para algoritmos de processamento de sinal digital (DSP) e aplicações. 

O `FFT` apresenta características que nos permite tirar proveito do paralelismo obtido através do uso de múltiplos processadores, como as diversas operações matemáticas e a geração de tabelas geométricas. Além disso, podemos fazer uso de pelo menos dois tipos de aceleradores, que serão discutidos abaixo.

## Kernel de Código ou Conjunto de Funções a ser Acelerado
Com o `FFT`, podemos utilizar dois tipos de aceleradores:
- Acelerador 1: operações de ponto flutuante.
- Acelerador 2: operações trigonométricas, tais como o cálculo de seno e cosseno.

## Estratégia para realização de experimentos
A fim de analisar o impacto de múltiplos cores, assim como o impacto do acelerador no desempenho do software, serão realizados 8 experimentos:
- Experimento 1: execução do software sem paralelismo e sem acelerador.
- Experimento 2: execução do software com 2 processadores e sem acelerador.
- Experimento 3: execução do software com 4 processadores e sem acelerador.
- Experimento 4: execução do software com 8 processadores e sem acelerador.
- Experimento 5: execução do software com melhor configuração de paralelismo e com acelerador 1.
- Experimento 6: execução do software com melhor configuração de paralelismo e com acelerador 2.
- Experimento 7: execução do software sem paralelismo e com acelerador 1 e 2.
- Experimento 8: execução do software com melhor configuração de paralelismo e com acelerador 1 e 2.

Desta forma, vamos inicialmente determinar a melhor configuração de paralelismo, e então verificar o impacto de cada acelerador individualmente, assim como o impacto deles em conjunto, com ou sem paralelismo.

## Cronograma
- Semana 1: Criação do roteiro, planejamento do cronograma, executar software sem paralelismo e identificar "hot spots" (áreas do código que demoram mais).
- Semana 2: Implementar múltiplos processadores, início da implementação dos aceleradores e início do relatório.
- Semana 3: Finalizar a implementação dos aceleradores, execução dos experimentos, análise dos resultados e desenvolvimento do relatório.
- Semana 4: Finalização da análise e do relatório e criação da apresentação.

