#ifndef starsky__polypreference
#define starsky__polypreference


#include "degreepreferencefunction.h"
#include <cmath>

namespace Starsky {

 /**
  * a preferential attachment function of the form:
  * \alpha k ^{\beta} + \gamma
  * where k is the degree
  */
class PolyPreference : public DegreePreferenceFunction {

    public:
        PolyPreference(double power = 1.0, double p0 = 0.0);
	double evaluate(int degree) const;
    protected:
	double _power;
	double _p0;
	
};
	
}

#endif
