#!/bin/sh

declare -a repos=("https://bitbucket.org/AlexandreMestre/projeto1.git"
"https://github.com/henriquefacioli/mc733benchmark.git"
"https://github.com/Guilhermeslucas/MC733-Projeto1.git" 
"https://github.com/LRacoci/projeto1-mc733"
"https://bitbucket.org/mc723/projeto1-mc723.git" 
"https://github.com/barleto/benchmark-mc723.git"
"https://bitbucket.org/gzanelato/mc723-projeto1.git" 
"https://bitbucket.org/MatheusKoez/mc723-projeto1.git"
"https://github.com/leocvf/Mc723-Grupo9-.git" 
"https://gitlab.com/mateusccoelho/benchmark-mc733.git"
"https://gitlab.com/guilhermesenazuza/mc733_projeto_1.git"
"https://gitlab.com/bszazulla/mc733-Projetos.git" 
"https://github.com/henriquelalves/mc723benchmarking.git"
"https://bitbucket.org/RodrigoNMaximo/projeto1.git" 
"https://github.com/gribeler/MC723-Project1.git"
"https://github.com/eKrakauer/projeto1" 
"https://github.com/laurocruz/t1-mc733"
"https://github.com/rafaelfariac/MC723-1s.2017.git")

index=0
for i in "${repos[@]}"
do
    index=$((index+1))

    cd bench$index
    git clone $i
    cd ..
done
