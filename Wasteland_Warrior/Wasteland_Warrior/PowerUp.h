#pragma once
#include "Entity.h"

class PowerUp: 
	public Entity
{
public:

	PowerUp();
	~PowerUp();

	int type;

	PowerUp(int type, float x, float y);
};

