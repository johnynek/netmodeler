#!/usr/bin/python

import sys
from pynetmod import *

size = int(sys.argv[1])
r = Ran1Random()
pl = PowerLawDRV(r,-2.0, 2, int(size ** 0.5))
net = DegreeLawNetFac(size, pl, r, False).create()
net.printTo(cout)
