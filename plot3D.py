import sys
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

colors = ["b", "g", "m", "r", "c", "y"]

# if len(sys.argv) != 2:
#     print("usage: python3 heatmap.py [dmd file]")

# filename = sys.argv[1]
filename = ".\\testcase\\case3.txt"

def getline():
    global filename
    with open(filename, "r") as f:
        lines = [x.replace("\n", "") for x in filter(None, f.readlines())]
    
    for line in lines:
        yield line

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

G = getline()

# get num of layers
while True:
    line = next(G).split()
    if line[0] == "NumLayer":
        layer_count = int(line[1])
        break

while True:
    line = next(G).split()
    if line[0] == "NumCellInst":
        ins_count = int(line[1])
        break

#draw cells (thick black vertical lines)
for i in range(ins_count):
    line = next(G).split()
    x = int(line[3])
    y = int(line[4])
    ax.plot([x, x], [y, y], [1, layer_count], color="k", linewidth=7)

#draw nets (thin lines, each with one color)
net_labels = {}
not_used = 0
while True:
    line = next(G).split()
    if line[0] == "NumRoutes":
        route_count = int(line[1])
        break

for i in range(route_count):
    data = next(G).split()
    label = data[-1]
    if label not in net_labels:
        net_labels[data[-1]] = not_used % 6
        not_used += 1
    data = [int(x) for x in data[:-1]]
    ax.plot([data[0], data[3]], [data[1], data[4]], [data[2], data[5]], color=colors[net_labels[label]])