import numpy as np

row = 3
col = 4
chans = 5

# matA = np.random.uniform(-20, 20, (row, col))
# matB = np.random.uniform(-30, 30, (row, col))
matA = np.random.uniform(-20, 20, (chans, row, col))
matB = np.random.uniform(-30, 30, (chans, col, row))

# np.savetxt("/home/zwxdogs/Cpp/Project2/data/matA_3_4.txt", matA, fmt='%.16f', delimiter=' ')
# np.savetxt("/home/zwxdogs/Cpp/Project2/data/matB_4_3.txt", matB, fmt='%.16f', delimiter=' ')

with open('/home/zwxdogs/Cpp/Project4/data/matA_3_4_5.txt', 'w') as outfile:
    for slice in matA:
        np.savetxt(outfile, slice, fmt='%.16f', delimiter=' ')
with open('/home/zwxdogs/Cpp/Project4/data/matB_4_3_5.txt', 'w') as outfile:
    for slice in matB:
        np.savetxt(outfile, slice, fmt='%.16f', delimiter=' ')