#pragma once
#include "Vehicle.h"


class PlayerUnit :
	public Vehicle
{
public:
	glm::vec3 heading;

	PlayerUnit();
	PlayerUnit(int newPhysicsIndex, int newSceneObjectIndex);
	~PlayerUnit();
};

