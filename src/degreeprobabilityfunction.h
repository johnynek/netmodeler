#ifndef starsky__degreeprobabilityfunction
#define starsky__degreeprobabilityfunction

namespace Starsky {

  /**
   * Abstract class for defining degree probability functions
   * for use in producing random networks with given degree
   * distributions.
   */
	
class DegreeProbabilityFunction {

    public:
	/**
	 * @return the probability that a node has a given degree
	 */
        virtual double getProbabilityOf(int degree) const = 0;
	/**
	 * @param prob a random cdf value (0.0,1.0)
	 * @return the smallest degree with cdf > prob
	 */
	virtual int getRandomDegree(double prob) const = 0;
	/**
	 * @return minimum degree with p > 0
	 */
	virtual int minDegree() const = 0;
	/**
	 * @return maximum degree with p > 0
	 */
	virtual int maxDegree() const = 0;
	
};
	
}

#endif
