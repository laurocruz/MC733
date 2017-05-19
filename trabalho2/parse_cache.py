#!/bin/python3

import glob
import re

names = ["basicmath_small", "fft_small", "fft_inv_small", "patricia_small"]

# programs
for filename in names:
    out = open("cache_csv/" + str(filename) + ".csv", 'w')

    # cenarios
    for j in range(11):
        # Cenarios de avaliação da cache
        if (j == 0 or (j >= 3 and j <= 9)):
            with open ("output/" + str(filename)+ str(j) +".txt", 'r') as fp:
                for i, line in enumerate(fp):
                    # L1 Data
                    if i in (1,2,5,8,9,12,16):
                        per = line.split()[-2]
                        out.write(per + ',')
                    # L2 Inst
                    elif i == 21:
                        per = line.split()[-2]
                        out.write(per + "\n")
