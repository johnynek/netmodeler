all : p2pmodeler spamgraph comgraph comdot graph_deg_dist hoodofmax \
	triangleman merger comtree compsplit

p2pmodeler : p2pmodeler.cpp ../netmodlib.a
	g++ -o p2pmodeler p2pmodeler.cpp -I../ ../netmodlib.a
graph_deg_dist : graph_deg_dist.cpp ../netmodlib.a
	g++ -O2 -o graph_deg_dist graph_deg_dist.cpp -I../ ../netmodlib.a
spamgraph : spamgraph.cpp ../netmodlib.a
	g++ -O2 -o spamgraph spamgraph.cpp -I../ ../netmodlib.a
comgraph : comgraph.cpp ../netmodlib.a
	g++ -O2 -o comgraph comgraph.cpp -I../ ../netmodlib.a
comdot : comdot.cpp ../netmodlib.a
	g++ -O2 -o comdot comdot.cpp -I../ ../netmodlib.a
hoodofmax : hoodofmax.cpp ../netmodlib.a
	g++ -O2 -o hoodofmax hoodofmax.cpp -I../ ../netmodlib.a
triangleman : triangleman.cpp ../netmodlib.a
	g++ -O2 -o triangleman triangleman.cpp -I../ ../netmodlib.a
merger : merger.cpp ../netmodlib.a
	g++ -O2 -o $@ $< -I../ ../netmodlib.a -lm
comtree : comtree.cpp ../netmodlib.a
	g++ -O2 -o $@ $< -I../ ../netmodlib.a -lm
compsplit : compsplit.cpp ../netmodlib.a
	g++ -O2 -o $@ $< -I../ ../netmodlib.a -lm
