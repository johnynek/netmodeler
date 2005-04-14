PROGS =  p2pmodeler spamgraph comgraph comdot graph_deg_dist hoodofmax \
	triangleman merger comtree compsplit bpsim
NMDIR = ../src
LIBNM = $(NMDIR)/libnetmod.a

all : $(PROGS)

% : %.cpp $(LIBNM)
	g++ -O2 -o $@ $< -I$(NMDIR) $(LIBNM) -lm
clean :
	rm $(PROGS)
