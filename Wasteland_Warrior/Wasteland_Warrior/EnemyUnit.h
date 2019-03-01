#pragma once
#include "Vehicle.h"
class EnemyUnit :
	public Vehicle
{
public:
	EnemyUnit();
	EnemyUnit(int newPhysicsIndex, int newSceneObjectIndex);
	~EnemyUnit();

	int type;
};

