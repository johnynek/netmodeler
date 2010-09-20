#!/usr/bin/env python

import sys
min_weight = float(sys.argv[1])

for line in sys.stdin:
	weight = float(line.split(" : ")[2])
	if weight >= min_weight:
		print line.strip()
