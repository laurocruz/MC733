#!/bin/bash

declare -a traces=("164.gzip.f2b" "176.gcc.m2b" "177.mesa.f2b" "254.gap.m2b")
declare -a tfiles=("gzip_f2b" "gcc_m2b" "mesa_f2b" "gap_m2b")
declare -a l1sizes=(2K 4K 8K 16K 32K)

export REPO="/home/ubuntu/code/MC733/exercicio2"

cd /home/ubuntu/benchmarks/traces
#cd /opt/mc723/benchmarks/traces

for ((i=0;i<${#traces[@]};++i)) 
do
	cd ${traces[i]}

	for l in "${l1sizes[@]}" 
	do
		dineroIV -informat s -trname ${tfiles[i]} -maxtrace 101 -l1-isize $l -l1-dsize $l -l1-ibsize 32 -l1-dbsize 32 >> $REPO/test1.txt
	done

	cd ../
done

