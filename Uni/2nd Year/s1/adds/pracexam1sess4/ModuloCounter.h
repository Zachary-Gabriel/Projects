/*  
    Base class for a counter
*/

#ifndef MODULO_COUNTER_H
#define MODULO_COUNTER_H

#include <string>
#include "Counter.h"

class ModuloCounter: public Counter{
 public:
  // Constructor - takes starting value and limit. If limit is reached counter goes back 
  // to zero. 
  ModuloCounter(int val,int limit);
  virtual void inc();

 protected:
  // field variable
  int limit;
};
#endif // MODULO_COUNTER_H
