#ifndef starsky__directededge_h
#define starsky__directededge_h

#include "edge.h"

//The below is to use the bad_cast exception, used in < and ==
#include <typeinfo>

namespace Starsky {

class DirectedEdge : public Edge {

    public: 
        DirectedEdge(Node* start=0, Node* end=0);
        virtual bool connects(Node* start, Node* end) const;
	Node* getStartNode() const;
	Node* getEndNode() const;
	bool pointsFirstToSecond() const { return _dir_first_to_second; }
	void reverse();

	std::string toString() const;
    
        bool operator<(const Edge& a) const; 
        bool operator==(const Edge& a) const;
    
    protected:
        bool _dir_first_to_second; // is true of the direction is from first to second
};
	
}

#endif
