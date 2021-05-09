#include "person.h"
#include "aircraft.h"
#include <string>

int main(void)
{
	person man = person("shit cunt",13);
	person woman = person("genetic failure",12);
	person optimus_prime = person("autobots, roll out",4000);

	aircraft plane = aircraft("car",man,woman);
	plane.printDetails();
	person p1 = plane.getPilot();
	person p2 = plane.getCoPilot();
	person p3 = optimus_prime;

	plane.setPilot(p3);
	plane.setCoPilot(p1);
	plane.printDetails();
	
	return 0;
}