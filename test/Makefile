PROGS = rewiretest partdisttest makenet stattest weightedtest fitertest
NMDIR = ../src
LIBNM = $(NMDIR)/libnetmod.a

all : $(PROGS) 

%.o : %.cpp %.h
	g++ -c $<

% : %.cpp $(LIBNM)
	g++ -O2 -o $@ $< -I$(NMDIR) $(LIBNM) -lm
clean :
	rm $(PROGS)
