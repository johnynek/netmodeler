#include "polypreference.h"

using namespace Starsky;
using namespace std;

PolyPreference::PolyPreference(double power,
			       double p0) : _power(power), _p0(p0) {
}

double PolyPreference::evaluate(int degree) const {

    return pow((double)degree, _power) + _p0;
}
