#pragma once
#include "Entity.h"
class Vehicle :
	public Entity
	 
{
public:
	Vehicle();
	~Vehicle();

	int playerDrivemode;

	//2D xy vector of the direction of the car
	glm::vec2 direction = glm::vec2(1.f, 0.f);

	float health;


	//Used for disableing AI vehicles
	int getActive();
	void setActive(int activeState);

	int active = 1;
};

