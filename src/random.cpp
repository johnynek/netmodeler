#include "random.h"

using namespace Starsky;

bool Random::getBool() {
  return (getDouble01() < _true_bias);
}

int Random::getInt() {
  return (int)(( (_imax + 1) - _imin) * getDouble01()) + _imin;
}

int Random::getInt(int high, int low) {
  if ( low > high)
    throw std::invalid_argument("RNG args out of order");
  return (int)(( (high + 1) - low) * getDouble01()) + low;
}

double Random::getDouble() {
  return (_dmax - _dmin) * getDouble01() + _dmin;
}

double Random::getDouble(double high, double low) {
  if ( low > high)
    throw std::invalid_argument("RNG args out of order");
  return (high - low) * getDouble01() + low;
}

void Random::setDoubleRange(double high, double low) {
  if ( low > high)
    throw std::invalid_argument("RNG args out of order");
    _dmax = high;
    _dmin = low;
}

void Random::setIntRange(int high, int low) {
  if ( low > high)
    throw std::invalid_argument("RNG args out of order");
	_imax = high;
  _imin = low;
}

void Random::setBoolTrueBias(double p) {
  _true_bias = p;
}
