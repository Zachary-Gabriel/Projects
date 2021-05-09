/*  
    Base class for a counter
*/

#ifndef COUNTER_H
#define COUNTER_H

#include <string>

class Counter{
 public:
  // Constructor - takes counters starting value
  Counter(int);

  // getter methods

  virtual int get_val();
  virtual void reset();
  virtual void inc();
 protected:
  // field variable
  int val;
};
#endif // COUNTER_H
