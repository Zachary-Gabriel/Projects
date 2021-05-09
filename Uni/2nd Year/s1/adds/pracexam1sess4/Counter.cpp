

#include "Counter.h"

// put implementation code for the constructor and functions here

  Counter::Counter(int v){
  	val = v;
  }

  int Counter::get_val(){
  	return val;
  }
  void Counter::reset(){
  	val = 0;
  }
  void Counter::inc(){
  	val++;
  }