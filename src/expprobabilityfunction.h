#ifndef starsky__expprobabilityfunction
#define starsky__expprobabilityfunction

#include "degreeprobabilityfunction.h"
#include <cmath>
#include <vector>

namespace Starsky {

/**
 * Degree distribution function for a exponential probability
 * distribution.
 */

class ExpProbabilityFunction : public DegreeProbabilityFunction {

    public:
	/**
	 * @param base that is exponentiated
	 * @param min_deg the minimum degree allowed
	 * @param max_deg the maximum degree allowed (defaults to (2^31-2), which is ~2*10^9)
	 * p_k = base^(-k) for all k between min_deg and max_deg
	 */
        ExpProbabilityFunction(double base = 0.5,
				    int min_deg = 1,
				    int max_deg = 0x7FFFFFFE);
	
	double getProbabilityOf(int deg) const;
	int getRandomDegree(double prob) const;
	int minDegree() const { return _min; }
	int maxDegree() const { return _max; }
    protected:
	double _base;
	double _coeff;
	double _coeff2;
	double _coeff3;
	int _min;
	int _max;
};
	
}

#endif
