#ifndef starsky__exppreference
#define starsky__exppreference


#include "degreepreferencefunction.h"
#include <cmath>

namespace Starsky {
 /**
  * a preferential attachment function of the form:
  *  exp { \beta k } + \gamma
  * where k is the degree
  */

class ExpPreference : public DegreePreferenceFunction {

    public:
        ExpPreference(double beta = 1.0, double p0 = 0.0);
	double evaluate(int degree) const;
    protected:
	double _beta;
	double _p0;
	
};
	
}

#endif
