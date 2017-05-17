#!/bin/bash

set -e

l1_s=(16 16 16 14 15 16 16 16 16 16 16 16 16 16) 
l1_bs=(5 5 5 5 5 5 5 4 5 5 5 5 5 5)
l2_s=(18 18 18 18 18 17 19 18 18 18 18 18 18 18 )
l2_bs=(7 7 7 7 7 7 7 7 6 7 7 7 7 7)
assoc=(2 2 2 2 2 2 2 2 2 1 2 2 2 2)

p_stage=(5 5 7 5 5 5 5 5 5 5 5 5 5 5)
p_scalar=(1 2 1 1 1 1 1 1 1 1 1 1 2 2)
branch=(0 0 0 0 0 0 0 0 0 0 1 2 1 2)

function realpath { echo $(cd $(dirname $1); pwd)/$(basename $1); }

BASEDIR=$(realpath $(dirname "$0"))

for (( i=0; i < ${#l1_s[@]}; ++i ))
do
    export MIPS_DEFINES="-D_L1_CACHE_LG2_SIZE=${l1_s[$i]} -D_L1_CACHE_LG2_B_SIZE=${l1_bs[$i]} \
        -D_L2_CACHE_LG2_SIZE=${l2_s[$i]} -D_L2_CACHE_LG2_B_SIZE=${l2_bs[$i]} \
        -D_CACHE_ASSOC=${assoc[$i]} -D_PIPELINE_SIZE=${p_stage[$i]} \
        -D_PIPELINE_TYPE=${p_scalar[$i]} -D_BRANCH_PRED=${branch[$i]}"

    make


    cd $BENCH/network/patricia
    #sudo chmod +x runme_small.sh
    ./runme_small.sh
    #$BENCH/network/patricia/runme_large.sh

    cd $BENCH/automotive/basicmath
    #sudo chmod +x runme_small.sh
    ./runme_small.sh
    #$BENCH/automotive/basicmath/runme_large.sh

    cd $BENCH/telecomm/FFT
    #sudo chmod +x runme_small.sh
    ./runme_small.sh
    #$BENCH/telecom/FFT/runme_large.sh

    cd $BASEDIR

    make clean
done
