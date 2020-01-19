#! /usr/bin/env python3

import numpy as np

n = 50000
nnz = n * 30

for k in range(nnz):
   i = np.random.randint(0, n)
   j = np.random.randint(0, n)
   v = np.random.poisson(20)
   print(i, j, v)
