PROGS =  p2pmodeler spamgraph comdot graph_deg_dist hoodofmax \
	triangleman merger comtree compsplit bpsim noisycom trianglefilter \
	thresher subgraph comtest
LIBPROGS = comgraph
LIBS = optionparser.o
NMDIR = ../src
LIBNM = $(NMDIR)/.libs/libnetmod.a

all : $(LIBS) $(LIBPROGS) $(PROGS) 

%.o : %.cpp %.h
	g++ -c $<

% : %.cpp $(LIBNM) $(LIBS)
	g++ -O2 -o $@ $< -I$(NMDIR) $(LIBNM) $(LIBS) -lm
clean :
	rm $(PROGS) *.o
