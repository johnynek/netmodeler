#ifndef starsky__dynamicnetwork
#define starsky__dynamicnetwork

#include "network.h"
#include "incrementable.h"

namespace Starsky {

 /**
  * This abstract class is used to model networks which have
  * dynamical graphs.  This includes growing networks and
  * any network that whose graph is time dependent.
  */
	
class DynamicNetwork : public Network,
                       public Incrementable {

};
	
}

#endif
