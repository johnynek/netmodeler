#ifndef starsky__linearpreference
#define starsky__linearpreference

#include "degreepreferencefunction.h"

namespace Starsky {

/**
 * This is a simple linear preference with optional linear bias:
 * ~ k + p0
 */
class LinearPreference : public DegreePreferenceFunction {

    public:
        LinearPreference(double p0 = 0.0);
        double evaluate(int degree) const;

    protected:
	double _p0;
	
};
	
}

#endif
