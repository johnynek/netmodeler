#!/usr/bin/python

from pynetmod import *
import sys

size = int(sys.argv[1])
r = Ran1Random()
pl = PowerLawDRV(r,-2.0, 2, int(size ** 0.5))
net = DegreeLawNetFac(size, pl, r, False).create()
np = NewmanCom()
parts = np.partition(net)
p = 0
for part in parts:
  part.printToFile("part%i" % p)
  p = p + 1

np.deletePartition(parts)
