#include "linearpreference.h"

using namespace Starsky;

LinearPreference::LinearPreference(double p0) : _p0(p0) { }

double LinearPreference::evaluate(int deg) const
{
    return ((double)deg) + _p0;
}
