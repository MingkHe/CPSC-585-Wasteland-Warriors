#pragma once
#include "Vehicle.h"


class PlayerUnit :
	public Vehicle
{
public:
	PlayerUnit();
	PlayerUnit(int newPhysicsIndex, int newSceneObjectIndex);
	~PlayerUnit();
};

