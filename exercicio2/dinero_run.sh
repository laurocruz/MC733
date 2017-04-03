#!/bin/sh

cd /opt/mc723/benchmarks/traces

declare -a traces=(

dineroIV -informat s -trname vortex_f2b -maxtrace 101 -l1-isize 16K -l1-dsize 16K \
    -l1-ibsize 32 -l1-dbsize 32 -l1-assoc 1
