#!/bin/python3

import glob
import re

names = ["basicmath_small", "fft_small", "fft_inv_small", "patricia_small"]

# programs
for filename in names:
    out_p = open("cache_csv/percentage/" + str(filename) + ".csv", 'w')
    out_n = open("cache_csv/num/" + str(filename) + ".csv", 'w')

    # cenarios
    for j in range(11):
        # Cenarios de avaliação da cache
        if (j == 0 or (j >= 3 and j <= 9)):
            with open ("output/" + str(filename)+ str(j) +".txt", 'r') as fp:
                for i, line in enumerate(fp):
                    if i in (1,2,5,8,9,12,16):
                        line = line.split()
                        per = line[-2]
                        n = line[1]
                        out_p.write(per + ',')
                        out_n.write(n + ',')

                    elif i == 21:
                        line = line.split()
                        per = line[-2]
                        n = line[1]
                        out_p.write(per + '\n')
                        out_n.write(n + '\n')
