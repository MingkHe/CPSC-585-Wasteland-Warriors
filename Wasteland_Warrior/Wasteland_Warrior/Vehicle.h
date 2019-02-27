#pragma once
#include "Entity.h"
class Vehicle :
	public Entity
	 
{
public:
	Vehicle();
	~Vehicle();

	int playerDrivemode;
	float health;
};

