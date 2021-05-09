#include "aircraft.h"
#include "person.h"
#include <string>
#include <iostream>

/*  std::string copilot;
	std::string pilot;
	std::aeroplane;
*/

aircraft::aircraft(std::string callsign, person thePilot, person theCoPilot){
	pilot = thePilot;
	copilot = theCoPilot;
	aeroplane = callsign;
}

void aircraft::setPilot(person thePilot){
	pilot = thePilot;
}

person aircraft::getPilot(){
	return pilot;
}

void aircraft::setCoPilot(person theCoPilot){
	copilot = theCoPilot;
}

person aircraft::getCoPilot(){
	return copilot;
}

void aircraft::printDetails(){
	std::cout << aeroplane << std::endl << pilot.getName() << std::endl << copilot.getName() << std::endl;
}