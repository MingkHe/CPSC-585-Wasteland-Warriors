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

	float speed;
	float acceleration;
	float health;
};

