#ifndef starsky__incrementable
#define starsky__incrementable

namespace Starsky {

/**
 * An interface for use to specify elements that
 * have a discrete time nature which can be incremented
 * a number of steps
 */
	
class Incrementable {

    public:
        virtual void incrementTime(int steps=1) = 0;

};
	
}

#endif
