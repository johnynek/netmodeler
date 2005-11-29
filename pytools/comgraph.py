#!/usr/bin/python

from pynetmod import *
import sys

#size = int(sys.argv[1])
#r = Ran1Random()
#pl = PowerLawDRV(r,-2.0, 2, int(size ** 0.5))
#net = DegreeLawNetFac(size, pl, r, False).create()
net = Network(cin)
cp = ComponentPart()
np = NewmanCom()
comps = cp.partition(net)
p = 0
for comp in comps:
  parts = np.partition(comp)
  for part in parts:
    part.printToFile("part%i" % p)
    p = p + 1
  np.deletePartition(parts)
cp.deletePartition(comps)
