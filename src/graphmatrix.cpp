#include "graphmatrix.h"

using namespace std;
using namespace Starsky;

GraphMatrix::GraphMatrix(const Network& network):_network(network)
{
}

EdgeDirection GraphMatrix::IncidenceMatrixElement(Node* i, Node* j)
{
  enum EdgeDirection ret_val = NO_EDGE;
  const Network::ConnectedNodePSet& neighbors = _network.getNeighbors(i);
  Network::ConnectedNodePSet::const_iterator k = neighbors.find( j );
  if( k != neighbors.end() ) 
  {
    ///@todo make sure this will work for directed graphs
    ret_val = UNDIR;
  }
  return ret_val;
}

double GraphMatrix::TransitionProbabilityMatrixElement(Node* i, Node* j)
{
  double prob = 0.0;
  enum EdgeDirection edge_relationship = IncidenceMatrixElement(i,j);
  if (UNDIR == edge_relationship)
  {
    const Network::ConnectedNodePSet& neighbors = _network.getNeighbors(i);
    prob = 1.0/( (double)(neighbors.size()) );
  }
  return prob;
}

void GraphMatrix::transitionProbabilityMatrixToAsciiFiles(
          std::string base_name, 
          std::vector<Node*> matrix_order)
{
  vector<Node*>::const_iterator rows;
  vector<Node*>::const_iterator cols;
  filebuf fb;
  fb.open (base_name.c_str(),ios::out);
  ostream out(&fb);
  
  vector<Node*>::const_iterator order_start = matrix_order.begin();
  for(rows = matrix_order.begin(); rows != matrix_order.end(); rows++) 
  {
    for (cols = matrix_order.begin(); cols != matrix_order.end(); cols++ )
    {
       TransitionProbabilityMatrixElement( *rows,*cols);
       out <<TransitionProbabilityMatrixElement( *rows,*cols)<< " ";  
    }      
    out << endl;
  }
}


void GraphMatrix::unitSquareIncidenceMatrixToAsciiFile(
          std::string base_name, 
          std::vector<Node*> matrix_order)
{
  vector<Node*>::const_iterator rows;
  vector<Node*>::const_iterator cols;
  double increment = 1.0/(double)(matrix_order.size());
  double xcoord = 0.0;
  double ycoord = 0.0;
  
  filebuf fb;
  fb.open (base_name.c_str(),ios::out);
  ostream out(&fb);
  out <<matrix_order.size() <<endl;      
  enum EdgeDirection edge_relationship;
  for(rows = matrix_order.begin(); rows != matrix_order.end(); rows++) 
  {
    for (cols = matrix_order.begin(); cols != matrix_order.end(); cols++ )
    {
      edge_relationship = IncidenceMatrixElement( *rows,*cols);
      if ( UNDIR == edge_relationship )
        out << xcoord<<"\t"<<ycoord<<endl;
      xcoord += increment;
    }
    xcoord = 0.0;
    ycoord += increment;
  }

}

void GraphMatrix::incidenceMatrixToAsciiFile(
          std::string base_name, 
          std::vector<Node*> matrix_order)
{
  vector<Node*>::const_iterator rows;
  vector<Node*>::const_iterator cols;
  filebuf fb;
  fb.open (base_name.c_str(),ios::out);
  ostream out(&fb);
  for(rows = matrix_order.begin(); rows != matrix_order.end(); rows++) 
  {
    for (cols = matrix_order.begin(); cols != matrix_order.end(); cols++ )
    {
      enum EdgeDirection edge_relationship = IncidenceMatrixElement( *rows,*cols);
      
      switch( edge_relationship)
      {
        case UNDIR:
          out << "1 ";  
          break;
        default:
          out << "0 ";
      }
    }      
    out << endl;
  }
}


