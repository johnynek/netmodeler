#ifndef starsky__random_h
#define starsky__random_h

#include <stdexcept>

namespace Starsky {

/**
 * an abstract class to subclass to get random numbers
 */
class Random {

  public:
  
    void setDoubleRange(double high=1.0, double low=0.0);
    virtual double getDouble();
    virtual double getDouble(double high, double low=0.0);
    virtual double getDouble01() = 0;

    void setIntRange(int high, int low=0);
    virtual int getInt();
    virtual int getInt(int high, int low=0);
    
    void setBoolTrueBias(double p);
    virtual bool getBool();
    virtual bool getBool(double p) {return (getDouble01() < p); }

  protected:
      int _imin, _imax;
      double _dmin, _dmax;
      double _true_bias;
};

}

#endif
