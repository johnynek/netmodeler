#include "powerlawprobabilityfunction.h"

using namespace Starsky;

PowerLawProbabilityFunction::PowerLawProbabilityFunction(double expon,
		                                         int min,
							 int max) : _expon(expon),
                                                                    _min(min),
								    _max(max)
{
    int i;
    double sum = 0.0;
    for(i = _min; i < _max; i++) {
        sum += pow((double)i, _expon);
    }
    _coeff = 1.0/sum;
}

double PowerLawProbabilityFunction::getProbabilityOf(int deg) const {
    if( (_min <= deg) && (deg <= _max) ) {
      return _coeff * pow((double)deg, _expon);
    }
    return 0.0;
}
int PowerLawProbabilityFunction::getRandomDegree(double prob) const {
    double p_over_c = prob / _coeff;
    int i = _min;
    double sum = pow((double)i, _expon);
    while( (sum < p_over_c) && ( i < _max) ) {
	++i;
        sum += pow((double)i, _expon);
    }
    return i;
}

int PowerLawProbabilityFunction::minDegree() const {
    return _min;
}

int PowerLawProbabilityFunction::maxDegree() const {
    return _max;
}

