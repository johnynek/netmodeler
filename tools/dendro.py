#! /usr/bin/env python
import sys
import re
import string
from StringIO import StringIO

# we build a tree to represent the dendrogram.
# This class represents each "split" in the tree
class JoinElement:
	def __init__ (self, leftchild, rightchild, value, step):
		self.leftChild = leftchild
		self.rightChild = rightchild
		self.step = step
		self.value = value
		self.daddy = None
	def toString (self):
		return "left: " + str(self.leftChild) + " right: " + str(self.rightChild) \
			+ " step: " + str(self.step)

# All the joins together form a tree which we
# want to represent as a dendrogram
class JoinTree:
	# Create a JoinElement and add it to the tree
	value_element = {}
	depth_of = {}
	roots = []
	#these are nodes that represent JOINS, not VALUE nodes
	join_nodes = []
	maxdepth = 0
	def addJoin (self, value1, value2, step):
		if value1 in self.value_element:
			je1 = self.value_element[value1]
			#We really want its mac-daddy
			this_depth = 0
			while je1.daddy != None:
				je1 = je1.daddy
				this_depth = this_depth + 1
			self.depth_of[je1] = this_depth
			if this_depth > self.maxdepth:
				self.maxdepth = this_depth
		else:
			je1 = JoinElement (None, None, value1, step)
			self.value_element[value1] = je1
			
		if value2 in self.value_element:
			je2 = self.value_element[value2]
			#We really want its mac-daddy
			this_depth = 0
			while je2.daddy != None:
				je2 = je2.daddy
				this_depth = this_depth + 1
			self.depth_of[je2] = this_depth
			if this_depth > self.maxdepth:
				self.maxdepth = this_depth
		else:
			je2 = JoinElement (None, None, value2, step)
			self.value_element[value2] = je2
		#now we have the JoinElements for this join.
		#These nodes are certainly node roots:
		if je1 in self.roots:
			self.roots.remove(je1)
		if je2 in self.roots:
			self.roots.remove(je2)
		jnode = JoinElement(je1, je2, None, step)
		self.join_nodes.append(jnode)
		self.roots.append(jnode)
		je1.daddy = jnode
		je2.daddy = jnode
	#Now that we have the whole dendrogram tree in memory, we can print it out properly
	def getDendrogram (self, je):
		posmap = {}
		jearray = []
		joins = []
		self.fillPosition (je, posmap, jearray, joins)
		xstring = StringIO()
		ystring = StringIO()
		for jnode in joins:
			#go through in the order we joined
			if jnode.leftChild.value != None:
				#this is a value element:
				yl = 0
			else:
				yl = jnode.leftChild.step + 1
			if jnode.rightChild.value != None:
				yr = 0
			else:
				yr = jnode.rightChild.step + 1
			#x position is easy:
			xl = posmap[jnode.leftChild];
			xr = posmap[jnode.rightChild];
			ys = jnode.step + 1
			xstring.write("%f %f %f %f; " % (xl, xl, xr, xr))
			ystring.write("%f %f %f %f; " % (yl, ys, ys, yr))
		return xstring.getvalue (), ystring.getvalue(), jearray 
	#this is a recursive function.  It calls itself to
	#fill the map from je -> position(float), and position(int)->childless je
	def fillPosition (self, je, posmap, jearray, joins):
		if je.value != None:
			#This is a value node!
			pos = len(jearray)
			jearray.append(je)
			posmap[je] = pos
		else:
			#this is a join node:
			joins.append(je)
			#First fill the position's below:
			self.fillPosition (je.leftChild, posmap, jearray, joins)
			self.fillPosition (je.rightChild, posmap, jearray, joins)
			#now, we know the positions of everything below us, we are the average:
			pos = (posmap[je.leftChild] + posmap[je.rightChild])/2.0
			posmap[je] = pos
		#we are done
#Here is the main routine

q = []
step = 0
tree = JoinTree ()
#the number to name mapping is the comments:
num2namre = re.compile(r"#\t(\d+)\t(.*)");
num2name = {}
for line in sys.stdin.readlines ():
	if line[0] == "#":
		#This is a comment line
		m = num2namre.match(line)
		if m:
			num2name[ int(m.group(1)) ] = m.group(2)
		continue
	line = string.strip (line)
	this_element = string.split (line, "\t")
	el1 = int (this_element[0])
	el2 = int (this_element[1])
	tree.addJoin(el1, el2, step)
	step = step + 1
	q.append (float (this_element[2]))
print "#maxdepth %i" % tree.maxdepth
#We have built the tree in memory, now walk it and print out the lines we need
xstring = StringIO()
ystring = StringIO()
xstring.write("x=[");
ystring.write("y=[");

first = True
step = 1
for je in tree.roots:
	if first == False:
		xstring.write("; ")	
		ystring.write("; ")	
	xstr, ystr, labs = tree.getDendrogram(je)
	xstring.write("%s" % xstr)
	ystring.write("%s" % ystr)
	first = False
#print it out
xstring.write("];");
ystring.write("];");
ticlabstring = StringIO()
ticposstring = StringIO()
ticlabstring.write("ticlab = [")
ticposstring.write("ticpos = [")

first = True
step = 0
for label in labs:
	if first == False:
		ticlabstring.write (", ")
		ticposstring.write (", ")
	ticlabstring.write (str (label.value))
	ticposstring.write (str (step))
	step = step + 1
	first = False
ticlabstring.write("];")
ticposstring.write("];")

print xstring.getvalue()
print ystring.getvalue()
print ticlabstring.getvalue ()
print ticposstring.getvalue ()
print """tics = sprintf(' "%g" %g,', [ticlab;ticpos]);"""
print """eval(["gset xtics (",tics(2:length(tics)-1),")"]);"""
print """plot(x',y',"b;;");"""

for label in labs:
	print "#%i -> %s" % (label.value, num2name[label.value])
