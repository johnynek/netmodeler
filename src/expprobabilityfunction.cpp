#include "expprobabilityfunction.h"

using namespace Starsky;

ExpProbabilityFunction::ExpProbabilityFunction(double base,
		                                         int min,
							 int max) : _base(base),
                                                                    _min(min),
								    _max(max)
{
  _coeff = (1.0 - _base)/( pow(_base, _min) - pow(_base, (_max + 1)));
  _coeff2 = (1.0 - _base) / (pow(_base, _min) * _coeff);
  _coeff3 = 1.0/log(_base);
}

double ExpProbabilityFunction::getProbabilityOf(int deg) const {
    if( (_min <= deg) && (deg <= _max) ) {
      return _coeff * pow(_base, deg);
    }
    return 0.0;
}
int ExpProbabilityFunction::getRandomDegree(double prob) const {
    return _min + floor( _coeff3 * log(1.0 - _coeff2 * prob) );
}
