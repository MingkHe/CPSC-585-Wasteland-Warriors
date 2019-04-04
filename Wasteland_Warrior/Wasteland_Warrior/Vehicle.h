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
	glm::vec3 direction = glm::vec3(1.f, 0.f, 0.0f);

	float health;
	float maxhealth;
	float armor = 0.0;

	//Used for disableing AI vehicles
	int getActive();
	void setActive(int activeState);

	int active = 1;
	float damageMultiplier = 1;
};

