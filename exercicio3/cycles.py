with open('cycles.txt', 'r') as f:
    i = 0;
    cycles = 0;
    for line in f:
        if i % 2 == 1:
            cycles = cycles + int(line.split(' ')[-1])
        i = i + 1

print(str(cycles))
