#ifndef starsky__ran1random_h
#define starsky__ran1random_h

 /**
  * Random number generator which wraps a classic random
  * number generator "ran1".
  * @see http://ciks.cbt.nist.gov/~bentz/flyash/node14.html
  */

#include "random.h"

namespace Starsky {

class Ran1Random : public Random {

  public:
      //Should be seeded with a negative integer
      Ran1Random( int seed = -1);
      double getDouble01();

  private:
      int _seed;
};

}

#endif
