#ifndef starsky__trustgraphmatrix_h
#define starsky__trustgraphmatrix_h

#include "directedweightednetwork.h"
#include "realcontentnetwork.h"
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
 
  class TrustGraphMatrix //: public GraphMatrix
  {
    public:      
    
      TrustGraphMatrix();    

      TrustGraphMatrix(const DirectedWeightedNetwork& network);
    
      /* Markov transition probability trust matrix
       * @param i the first node
       * @param j the second node
       * @return the transition probability of going from node i to node j
       * as defined by edge_weight(i,j)/out_strength(i), value must be between 0 & 1.
       */
      double TrustMatrixElement(Node* i, Node* j);

      double TrustMatrixElement(Node* i, Node* j, RealContentNetwork* rc_net, std::set<Node*> pre_trusted);
     
      /* Writes the trust transition probability matrix to ascii file(s) 
       * 1 matrix row per line
       * 
       * @param base_name file name used for writing
       * 
       */
      void TrustMatrixToAsciiFile(
          std::string base_name,
          std::vector<Node*> matrix_order);

      /* Writes the principal eigenvector of a Markov chain, by implementing the power iteration method
       * @param matrix_order the transition probability matrix of the Markov Chain (trust matrix)
       * the elements of the matrix is defined by the TrustMatrixElement function above
       * @param pre_trusted the set of pre_trusted nodes (if a node has out-degree 0, it will trust the 
       * pre_trusted nodes with uniform probability, also at every iteration, a node will place a fraction of
       * trust to the pre_trusted nodes
       * @param eps the power iteration method will stop once the delta is less than eps
       * @return the principal eigenvector of the Markov chain
       */
      std::vector<double>  getPrincipalEigVector(std::vector<Node*> matrix_order, std::set<Node*> pre_trusted,
					    RealContentNetwork* rc_net, const double eps);     

      /* Same as the above functioin, but calculation is done in the log domain
       */
      std::vector<double>  getPrincipalEigVectorLog(std::vector<Node*> matrix_order, std::set<Node*> pre_trusted, const double eps);      

      double logadd(double num1, double num2);

      double logminus(double num1, double num2);

    protected:
      const DirectedWeightedNetwork& _network; 

  };
}

#endif


