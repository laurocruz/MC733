#!/bin/python3

import glob
import re

names = ["basicmath_small", "fft_inv_small", "fft_small", "patricia_small"]

for filename in names:
    for i in range(14):
        fp = open("../output/" + str(filename)+ str(i) +".txt")
        #fft = []
        #fft_inv = []
        #basic = []
        #patric = []
        ## Torna todos os dados únicos
        #if "basicmath_small" in filename:
        #    lines = fp.readlines()[19733:]
        #elif "fft_inv_small" in filename:
        #    lines = fp.readlines()[4:]
        #elif "fft_small" in filename:
        #    lines = fp.readlines()[4:]
        #elif "patricia_small" in filename:
        #    lines = fp.readlines()[10440:]

        print("\n")
        print(str(filename)+ ' ' + str(i) )

        for i, line in enumerate(fp):
            # L1 Data
            if i == 1:
                line = " ".join(line.split()).split(' ')
                #print("Data L1 Read, Data L1 Total Read, Data L1 Read %")
                #print(line[1] + ',' + line[3] + ',' + line[5])
                print("Data L1 Read," + line[1])
                print("Data L1 Total Read," + line[3])
            elif i == 2:
                line = " ".join(line.split()).split(' ')
                #print("Data L1 Write, Data L1 Total Write, Data L1 Write %")
                #print(line[1] + ',' + line[3] + ',' + line[5])
                print("Data L1 Write," + line[1])
                print("Data L1 Total Write," + line[3])
            elif i == 3:
                line = " ".join(line.split()).split(' ')
                #print("Data L1 Read+PF, Data L1 Total Read+PF, Data L1 Read+PF %")
                #print(line[1] + ',' + line[3] + ',' + line[5])
            elif i == 4:
                line = " ".join(line.split()).split(' ')
                #print("Data L1 Write+PF, Data L1 Total Write+PF, Data L1 Write+PF %")
                #print(line[1] + ',' + line[3] + ',' + line[5])
            elif i == 5:
                line = " ".join(line.split()).split(' ')
                #print("Data L1 TOTAL, Data L1 Total TOTAL, Data L1 TOTAL %")
                #print(line[1] + ',' + line[3] + ',' + line[5])

            # L2 Data
            elif i == 8:
                line = " ".join(line.split()).split(' ')
                #print("Data L2 Read, Data L2 Total Read, Data L2 Read %")
                #print(line[1] + ',' + line[3] + ',' + line[5])
                print("Data L2 Read," + line[1])
                print("Data L2 Total Read," + line[3])
            elif i == 9:
                line = " ".join(line.split()).split(' ')
                #print("Data L2 Write, Data L2 Total Write, Data L2 Write %")
                #print(line[1] + ',' + line[3] + ',' + line[5])
                print("Data L2 Write," + line[1])
                print("Data L2 Total Write," + line[3])
            elif i == 10:
                line = " ".join(line.split()).split(' ')
                #print("Data L2 Read+PF, Data L2 Total Read+PF, Data L2 Read+PF %")
                #print(line[1] + ',' + line[3] + ',' + line[5])
            elif i == 11:
                line = " ".join(line.split()).split(' ')
                #print("Data L2 Write+PF, Data L2 Total Write+PF, Data L2 Write+PF %")
                #print(line[1] + ',' + line[3] + ',' + line[5])
            elif i == 12:
                line = " ".join(line.split()).split(' ')
                #print("Data L2 TOTAL, Data L2 Total TOTAL, Data L2 TOTAL %")
                #print(line[1] + ',' + line[3] + ',' + line[5])

            # L1 Inst
            elif i == 16:
                line = " ".join(line.split()).split(' ')
                #print("Instruction L1, Instruction L1 Total, Instruction L1 %")
                #print(line[1] + ',' + line[3] + ',' + line[5])
                print("Instruction L1," + line[1])
                print("Instruction L1 Total," + line[3])
            elif i == 17:
                line = " ".join(line.split()).split(' ')
                #print("Instruction+PR L1, Instruction+PR L1 Total, Instruction+PR L1 %")
                #print(line[1] + ',' + line[3] + ',' + line[5])
            elif i == 18:
                line = " ".join(line.split()).split(' ')
                #print("Instruction Total, Instruction Total L1 Total, Instruction Total L1 %")
                #print(line[1] + ',' + line[3] + ',' + line[5])

            # L2 Inst
            elif i == 21:
                line = " ".join(line.split()).split(' ')
                #print("Instruction L1, Instruction L1 Total, Instruction L1 %")
                #print(line[1] + ',' + line[3] + ',' + line[5])
                print("Instruction L2," + line[1])
                print("Instruction L2 Total," + line[3])
            elif i == 22:
                line = " ".join(line.split()).split(' ')
                #print("Instruction+PR L1, Instruction+PR L1 Total, Instruction+PR L1 %")
                #print(line[1] + ',' + line[3] + ',' + line[5])
            elif i == 23:
                line = " ".join(line.split()).split(' ')
                #print("Instruction Total, Instruction Total L1 Total, Instruction Total L1 %")
                #print(line[1] + ',' + line[3] + ',' + line[5])

            # Others infos
            elif i == 26:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + ',' + str(float(line[1])))
            elif i == 27:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + ',' + str(float(line[1])))
            elif i == 28:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + line[1] +  ',' + str(float(line[2])))
            elif i == 29:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + line[1] +  ',' + str(float(line[2])))
            elif i == 30:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + line[1] +  ',' + str(float(line[2])))
            elif i == 31:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + ',' + str(float(line[1])))
            elif i == 32:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + line[1] +  ',' + str(float(line[2])))
            elif i == 33:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + line[1] +  ',' + str(float(line[2])))
            elif i == 34:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + ',' + str(float(line[1])))
            elif i == 35:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + line[1] +  ',' + str(float(line[2])))
            elif i == 36:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + line[1] +  ',' + str(float(line[2])))
            elif i == 37:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + line[1] +  ',' + str(float(line[2])))
            elif i == 38:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + ',' + str(float(line[1])))
            elif i == 39:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + line[1] +  ',' + str(float(line[2])))
            elif i == 40:
                line = line.replace(':', ' ')
                line = " ".join(line.split()).split(' ')
                print(line[0] + line[1] +  ',' + str(float(line[2])))
