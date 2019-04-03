#include "Vehicle.h"



Vehicle::Vehicle()
{
	this->health = 100;
}


Vehicle::~Vehicle()
{
}


int Vehicle::getActive() {
	return active;
}

void Vehicle::setActive(int activeState) {
	active = activeState;
}