#ifndef starsky__powerlawprobabilityfunction
#define starsky__powerlawprobabilityfunction

#include "degreeprobabilityfunction.h"
#include <cmath>
#include <vector>

namespace Starsky {

/**
 * Degree distribution function for a power-law with a given
 * exponent, minimum degree, and maximum degree.
 */

class PowerLawProbabilityFunction : public DegreeProbabilityFunction {

    public:
	/**
	 * @param exponent the exponent of the power law.
	 * @param min_deg the minimum degree allowed
	 * @param max_deg the maximum degree allowed (defaults to (2^31-1), which is ~2*10^9)
	 */
        PowerLawProbabilityFunction(double exponent = -2.0,
				    int min_deg = 1,
				    int max_deg = 0x7FFFFFFF);
	
	double getProbabilityOf(int deg) const;
	int getRandomDegree(double prob) const;
	int minDegree() const;
	int maxDegree() const;
    protected:
	double _expon;
	double _coeff;
	int _min;
	int _max;
	//The below is used to do efficient look up of the cdf of a given degree
	std::vector<int> _frac_to_deg;
	std::vector<double> _frac_to_cdf;
};
	
}

#endif
