#pragma once
#include "Entity.h"

class PowerUp: 
	public Entity
{
public:

	PowerUp();
	~PowerUp();

	int type;//Health Boost/ Accelereration Boost/ Max Speed Boost

	PowerUp(int type, float x, float y);
};