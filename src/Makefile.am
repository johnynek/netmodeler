.PHONY : netmodeler.h

netmodeler.h : 
	rm -f netmodeler.h;
	ls *.h | sed -e 's/\(.*\)/#include "\1"/' > netmodeler.h

noinst_LIBRARIES = libnetmod.a
noinst_PROGRAMS = netmodeler
netmodeler_SOURCES = main.cpp
netmodeler_LDADD = libnetmod.a
libnetmod_a_SOURCES = amnesiacmessage.cpp amnesiacmessage.h \
agglompart.h agglompart.cpp \
anycastmessage.cpp \
anycastmessage.h \
broadcastmessage.h \
broadcastmessage.cpp \
cache.h \
cache.cpp \
callawaynetwork.cpp \
callawaynetwork.h \
contentnetwork.cpp \
contentnetwork.h \
contentnode.h \
componentpart.h componentpart.cpp \
debianaliasnode.h \
debiandependencynetwork.cpp \
debiandependencynetwork.h \
debiannode.cpp \
debiannode.h \
debianpackagenode.cpp \
debianpackagenode.h \
degreelawrandomnetwork.cpp \
degreelawrandomnetwork.h \
degreelawnetfac.h degreelawnetfac.cpp \
degreepreferencefunction.h \
degreeprobabilityfunction.h \
directededge.cpp \
directededge.h \
directednetwork.cpp \
directednetwork.h \
directedweightednetwork.cpp \
directedweightednetwork.h \
doublembnet.h \
doublembnet.cpp \
doubleprefatnetwork.cpp \
doubleprefatnetwork.h \
dynamicnetwork.h \
edge.cpp \
edge.h \
edgefactory.h \
edgeiterator.h \
edgerewirer.h edgerewirer.cpp \
edgeswapper.h edgeswapper.cpp \
emailnetwork.cpp \
emailnetwork.h \
exppreference.cpp \
exppreference.h \
expprobabilityfunction.h \
expprobabilityfunction.cpp \
gnutellacontent.cpp \
gnutellacontent.h \
gnutellafilespacenetwork.cpp \
gnutellafilespacenetwork.h \
gnutellanetwork.cpp \
gnutellanetwork.h \
gnutellanode.cpp \
gnutellanode.h \
gnutellaquery.cpp  gnutellaquery.h \
graphmatrix.cpp graphmatrix.h \
incrementable.h \
inetworkpartitioner.h inetworkpartitioner.cpp \
inetworkmapper.h \
inetworkmonitor.h \
inodeselector.h \
iterator.h \
linearpreference.cpp \
linearpreference.h \
magnetmessage.h \
magnetmessage.cpp \
mergenetwork.h \
mergenetwork.cpp \
messagebuiltnetwork.cpp \
messagebuiltnetwork.h \
message.cpp \
message.h \
mrucache.cpp \
mrucache.h \
network.cpp \
network.h \
networkfactory.h networkfactory.cpp \
newmancom.h newmancom.cpp \
nullpart.h \
node.h \
node.cpp \
nodefactory.h nodefactory.cpp \
nodeiterator.h \
noisystardnet.h \
noisystardnet.cpp \
npartitenetwork.cpp  npartitenetwork.h \
percolationmessage.cpp \
percolationmessage.h \
percolationmapper.h percolationmapper.cpp \
polypreference.cpp \
polypreference.h \
powerlawprobabilityfunction.cpp \
powerlawprobabilityfunction.h \
prefatnetwork.cpp \
prefatnetwork.h \
prefatfunction.cpp \
prefatfunction.h \
prefdelcompnetwork.cpp \
prefdelcompnetwork.h \
prefdelcompattnetwork.cpp \
prefdelcompattnetwork.h \
ran1random.cpp \
ran1random.h \
random.cpp \
randomdirectednetwork.cpp \
randomdirectednetwork.h \
random.h \
randagpart.h randagpart.cpp \
randomnetwork.cpp \
randomnetwork.h \
realcontentnetwork.cpp \
realcontentnetwork.h \
realnode.cpp \
realnode.h \
simkinnetwork.cpp \
simkinnetwork.h \
simplerulenet.h \
simplerulenet.cpp \
sitepercmessage.h \
sitepercmessage.cpp \
smallworldnetwork.cpp \
smallworldnetwork.h \
spatialnetwork.cpp \
spatialnetwork.h \
spatialnode.cpp \
spatialnode.h \
superstring.cpp \
superstring.h \
trianglemapper.h \
trianglemapper.cpp \
trustgraphmatrix.h \
trustgraphmatrix.cpp \
uniformattachnet.h \
uniformattachnet.cpp \
uniformnodeselector.h uniformnodeselector.cpp \
vazquezdirectednetwork.cpp \
vazquezdirectednetwork.h \
walkandpercmessage.cpp \
walkandpercmessage.h \
walkandsitepercmessage.cpp \
walkandsitepercmessage.h \
weightednetwork.h \
weightednetwork.cpp \
weightednetworkfactory.h weightednetworkfactory.cpp


