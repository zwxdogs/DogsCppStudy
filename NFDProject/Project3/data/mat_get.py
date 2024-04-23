import numpy as np

row = 1024
col = 1024

# matA = np.random.uniform(-20, 20, (row, col))
# matB = np.random.uniform(-30, 30, (row, col))
matA = np.random.uniform(-20, 20, (row, col))
matB = np.random.uniform(-30, 30, (col, row))

np.savetxt("/home/zwxdogs/Cpp/Project2/data/matA_1024.txt", matA, fmt='%.16f', delimiter=' ')
np.savetxt("/home/zwxdogs/Cpp/Project2/data/matB_1024.txt", matB, fmt='%.16f', delimiter=' ')