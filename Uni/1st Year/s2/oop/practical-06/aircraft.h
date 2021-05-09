#ifndef aircraft_h
#define aircraft_h

#include "person.h"

class aircraft 
{
	person copilot;
	person pilot;
	std::string aeroplane;
public:
	// a pilot and copilot must be provided when creating an aircraft
	aircraft(std::string callsign,person thePilot,person theCoPilot);
	void setPilot(person thePilot);// change the pilot
	person getPilot();
	void setCoPilot(person theCoPilot);// change the co-pilot
	person getCoPilot();
	void printDetails();  // print the callsign, a new line, the pilot name,
 	// a new line, the co-pilot name and a final newline.
};

#endif