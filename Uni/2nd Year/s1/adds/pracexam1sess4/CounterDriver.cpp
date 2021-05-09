#include "Counter.h"
#include "ModuloCounter.h"
#include "Modulo3Counter.h"


#include<iostream>

int main(){
  // driver program

  Counter c = Counter(5);
  ModuloCounter d = ModuloCounter(7,4);
  Modulo3Counter e = Modulo3Counter(8);
  Counter* f = &d;
  Counter* g = &e; 
  
  // now print info for each

  std::cout << c.get_val()  << std::endl; // expect 5
  std::cout << d.get_val()  << std::endl; // expect 3
  std::cout << e.get_val()  << std::endl; // expect 2
  std::cout << f->get_val()  << std::endl; //expect 3
  std::cout << g->get_val()  << std::endl; // expect 2
  std::cout << "-------" << std::endl;
  
  // now increment 
  c.inc(); // expect 6
  c.inc(); // expect 7
  d.inc(); // expect 0 (because mod 4)
  e.inc(); // expect 0 (because mod 3)
  f->inc(); // expect 1 (alias of d)
  f->inc(); // expect 2 (alias of d)
  g->inc(); // expect 1 (alias of e)
  
  // print out again
  // now print info for each

  std::cout << c.get_val()  << std::endl; // expect 7
  std::cout << d.get_val()  << std::endl; // expect 2
  std::cout << e.get_val()  << std::endl; // expect 1
  std::cout << f->get_val()  << std::endl; // expect 2
  std::cout << g->get_val()  << std::endl; // expect 1
  std::cout << "-------" << std::endl;
  
  // now do object slicing.
  c=*(f);  // expect 2
  // now increment 
  c.inc(); // expect 3
  c.inc(); // expect 4
  c.inc(); // expect 5
  f->inc(); // expect 3
  
  
  // print out again
  // now print info for each

  std::cout << c.get_val()  << std::endl; // expect 5
  std::cout << d.get_val()  << std::endl;  // expect 3
  std::cout << e.get_val()  << std::endl;  // expect 1
  std::cout << f->get_val()  << std::endl;  // expect 3
  std::cout << g->get_val()  << std::endl;  // expect 1
  
  // reset some
  
  c.reset();
  d.reset();
  
  
  std::cout << "-------" << std::endl;
  std::cout << c.get_val()  << std::endl; // expect 0
  std::cout << d.get_val()  << std::endl;  // expect 0
  
 

}
