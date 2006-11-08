PROGS = rewiretest partdisttest makenet stattest weightedtest fitertest undist deetoo_test
NMDIR = ../src
LIBNM = $(NMDIR)/.libs/libnetmod.a

all : $(PROGS) 

%.o : %.cpp %.h
	g++ -c $<

% : %.cpp $(LIBNM)
	g++ -O2 -o $@ $< -I$(NMDIR) $(LIBNM) -lm
clean :
	rm $(PROGS)
