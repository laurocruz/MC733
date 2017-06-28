i = 0
p = []
with open("total_instrucoes.txt", 'r') as f:
    for l in f:
        i = i + 1
        if i is 3:
            i = 0
            p.append(eval(l))

print("(2S) em relacao a (1S):")
m = 100*(1-p[2]/p[0])
print("%.2f" % round(m,2) + "%", end="\n\n")

print("(4S) em relacao a (1S):")
m = 100*(1-p[4]/p[0])
print("%.2f" % round(m,2) + "%", end="\n\n")

print("(4S) em relacao a (2S):")
m = 100*(1-p[4]/p[2])
print("%.2f" % round(m,2) + "%", end="\n\n")

print("(8S) em relacao a (1S):")
m = 100*(1-p[6]/p[0])
print("%.2f" % round(m,2) + "%", end="\n\n")

print("(8S) em relacao a (4S):")
m = 100*(1-p[6]/p[4])
print("%.2f" % round(m,2) + "%", end="\n\n\n")


print("(1P) em relacao a (1S):")
m = 100*(1-p[1]/p[0])
print("%.2f" % round(m,2) + "%", end="\n\n")

print("(2P) em relacao a (2S):")
m = 100*(1-p[3]/p[2])
print("%.2f" % round(m,2) + "%", end="\n\n")

print("(4P) em relacao a (4S):")
m = 100*(1-p[5]/p[4])
print("%.2f" % round(m,2) + "%", end="\n\n")

print("(8P) em relacao a (8S):")
m = 100*(1-p[7]/p[6])
print("%.2f" % round(m,2) + "%", end="\n\n\n")


print("(2P) em relacao a (1P):")
m = 100*(1-p[3]/p[1])
print("%.2f" % round(m,2) + "%", end="\n\n")

print("(4P) em relacao a (1P):")
m = 100*(1-p[5]/p[1])
print("%.2f" % round(m,2) + "%", end="\n\n")

print("(4P) em relacao a (2P):")
m = 100*(1-p[5]/p[3])
print("%.2f" % round(m,2) + "%", end="\n\n")

print("(8P) em relacao a (1P):")
m = 100*(1-p[7]/p[1])
print("%.2f" % round(m,2) + "%", end="\n\n")

print("(8P) em relacao a (4P):")
m = 100*(1-p[7]/p[5])
print("%.2f" % round(m,2) + "%")

