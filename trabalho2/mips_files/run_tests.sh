#!/bin/bash

l1_s=(16 16 16 14 15 16 16 16 16 16 16 16 16 16) 
l1_bs=(5 5 5 5 5 5 5 4 5 5 5 5 5 5)
l2_s=(18 18 18 18 18 17 19 18 18 18 18 18 18 18 )
l2_bs=(7 7 7 7 7 7 7 7 6 7 7 7 7 7)
assoc=(2 2 2 2 2 2 2 2 2 1 2 2 2 2)

p_stage=(5 5 7 5 5 5 5 5 5 5 5 5 5 5)
p_scalar=(1 2 1 1 1 1 1 1 1 1 1 1 2 2)
branch=(0 0 0 0 0 0 0 0 0 0 1 2 1 2)

function realpath { echo $(cd $(dirname $1); pwd)/$(basename $1); }

mkdir -p output

BASEDIR=$(realpath $(dirname "$0"))
OUTPUT_DIR=$(realpath output)

for (( i=0; i < ${#l1_s[@]}; ++i ))
do
    export MIPS_DEFINES="-D_L1_CACHE_LG2_SIZE=${l1_s[$i]} -D_L1_CACHE_LG2_B_SIZE=${l1_bs[$i]} \
        -D_L2_CACHE_LG2_SIZE=${l2_s[$i]} -D_L2_CACHE_LG2_B_SIZE=${l2_bs[$i]} \
        -D_CACHE_ASSOC=${assoc[$i]} -D_PIPELINE_SIZE=${p_stage[$i]} \
        -D_PIPELINE_TYPE=${p_scalar[$i]} -D_BRANCH_PRED=${branch[$i]}"

    make

    cd $BENCH/network/patricia
    ${SIMULATOR}patricia small.udp > $OUTPUT_DIR/patricia_small$i.out

    cd $BENCH/automotive/basicmath
    ${SIMULATOR}basicmath_small > $OUTPUT_DIR/basicmath_small$i.out

    cd $BENCH/telecomm/FFT
    ${SIMULATOR}fft 4 4096 > $OUTPUT_DIR/fft_small$i.out
    ${SIMULATOR}fft 4 8192 > $OUTPUT_DIR/fft_inv_small$i.out

    cd $BASEDIR

    make clean
done
