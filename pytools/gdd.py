#!/usr/bin/python

#This is a python version of the graph_degree_dist.cpp code:

from pynetmod import *

net = Network(cin)
print "#loaded network"
ns = IntStats()
ns.collectN(net,"getDegree")
ns.collectByEdge(net, "getDegree")

print "#assortativity: %f" % (ns.getEdgeCorrelation())
print "#cluster coeff: %f" % (net.getClusterCoefficient())
print "#transitivity: %f" % (net.getTransitivity())
print "#nodes: %i" % (net.getNodeSize())
print "#edges: %i" % (net.getEdgeSize())
print "#<k>: %f" % (ns.getAverage())
print "#<k^2>: %f" %  (ns.getMoment(2.0))
print "#H(degs): %f" % (ns.getEntropy())
(h1, h2, h3) = ns.getEdgeEntropy();
print "#H(e_i): %f\n#H(e_ij): %f" % (h1, h3)
print "#EdgeMI: %f" % ns.getEdgeMutualInfo()
	
#Print out the degree distribution:
print "#printing out degree distribution"
for (deg, count) in ns.getDist().iteritems():
  print "%i %i" % (deg, count)
