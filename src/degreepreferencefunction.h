#ifndef starsky__degreepreferencefunction
#define starsky__degreepreferencefunction


namespace Starsky {

 /**
  * Abstract class for defining preference functions
  * for the preferential attachment networks.
  */
class DegreePreferenceFunction {

    public:
        virtual double evaluate(int degree) const =0;
	
};
	
}

#endif
