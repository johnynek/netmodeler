#include "exppreference.h"

using namespace Starsky;

ExpPreference::ExpPreference(double beta,
			       double p0) : _beta(beta), _p0(p0) {
}

double ExpPreference::evaluate(int degree) const {

    return exp(_beta * (double)degree) + _p0;
}
