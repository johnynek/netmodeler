/*
This program is part of Netmodeler, a library for graph and network
modeling and simulation.
Copyright (C) 2005  University of California
Copyright (C) 2005  P. Oscar Boykin <boykin@pobox.com>, University of Florida

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "graphmatrix.h"

using namespace std;
using namespace Starsky;

GraphMatrix::GraphMatrix(const Network& network):_network(network)
{
}

EdgeDirection GraphMatrix::IncidenceMatrixElement(Node* i, Node* j)
{
  enum EdgeDirection ret_val = NO_EDGE;
  Edge* e = _network.getEdge(i,j);
  if( e != 0 ) 
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
    prob = 1.0/( (double)_network.getDegree(i));
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


