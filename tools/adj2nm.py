#!/usr/bin/env python

# This is a simple program that converts adjaceny matrices
# to netmodeler format:

import sys

node = 1;
for line in sys.stdin:
  vals = line.rstrip().split(" ");
  target = 1
  for val in vals:
    if int(val) == 1 and target > node:
      print "n%i : n%i" % (node,target)
    target = target + 1
  node = node + 1
