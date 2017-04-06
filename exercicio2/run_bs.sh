#!/bin/bash

declare -a traces=("164.gzip.f2b" "176.gcc.m2b" "177.mesa.f2b" "254.gap.m2b")
declare -a tfiles=("gzip_f2b" "gcc_m2b" "mesa_f2b" "gap_m2b")
declare -a bsizes=(2 4 8 16 32)

export REPO="/home/ubuntu/MC733/exercicio2"

cd /home/ubuntu/benchmarks/traces
#cd /opt/mc723/benchmarks/traces

for ((i=0;i<${#traces[@]};++i)) 
do
	cd ${traces[i]}

	for l in "${bsizes[@]}" 
	do
		dineroIV -informat s -trname ${tfiles[i]} -maxtrace 101 -l1-isize 32K -l1-dsize 32K -l1-ibsize $l -l1-dbsize $l >> $REPO/test2.txt
	done

	cd ../
done

