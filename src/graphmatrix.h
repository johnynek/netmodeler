#ifndef starsky__graphmatrix_h
#define starsky__graphmatrix_h

#include "network.h"
#include "node.h"
#include "edge.h"
#include "superstring.h"
#include <set>
#include <map>
#include <queue>
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>

namespace Starsky
{
  enum EdgeDirection { DIR_I_TO_J =1, DIR_J_TO_I =-1, UNDIR =2 ,NO_EDGE =0};
  
  class GraphMatrix
  {
    public:
      
    
      GraphMatrix();
    
    
      GraphMatrix(const Network& network);

    
      /* Incidence matrix
       * @param i the first node
       * @param j the second node
       * @return the enum representing the edge relationship
       */
      EdgeDirection IncidenceMatrixElement(Node* i, Node* j);
    
      /* Markov transition probability matrix
       * @param i the first node
       * @param j the second node
       * @return the transition probability of going from node i to node j.
       */
      double TransitionProbabilityMatrixElement(Node* i, Node* j);

      /* Writes the incidence matrix to ascii file 
       * 1 matrix row per line
       * For undirected graphs the upper triangular matrix is reflected about
       * the diagonal and all matrix elements are 1
       * @todo should this value be the edge weight?
       * 
       * @param base_name file name used for writing
       * @param matrix_order determines which permutation of the incidence
       * matrix will be written.
       */
      void incidenceMatrixToAsciiFile(
          std::string base_name, 
          std::vector<Node*> matrix_order);
     
      
      /* Writes the incidence matrix to ascii file for use with FD3
       * The matrix is mapped onto the unit square to aid analysis of any
       * geometrical structure in the matrix(i.e. fractal).
       * 1 matrix row per line
       * For undirected graphs the upper triangular matrix is reflected about
       * the diagonal and all matrix elements are 1
       * @todo should this value be the edge weight?
       * 
       * @param base_name file name used for writing
       * @param matrix_order determines which permutation of the incidence
       * matrix will be written.
       */
      void unitSquareIncidenceMatrixToAsciiFile(
          std::string base_name, 
          std::vector<Node*> matrix_order);
      
      /* Writes the transition probability matrix to ascii file(s) 
       * 1 matrix row per line
       * 
       * @todo
       * For multiple graph components multiple files will be produced
       * 
       * @param base_name file name used for writing
       * 
       */
      void transitionProbabilityMatrixToAsciiFiles(
          std::string base_name,
          std::vector<Node*> matrix_order);

    protected:
      const Network& _network; 

  };
}

#endif
