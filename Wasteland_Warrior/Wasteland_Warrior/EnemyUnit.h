#pragma once
#include "Vehicle.h"

//Time stuff
#include <sys/timeb.h>
#include <stdio.h>
#include <errno.h>

class EnemyUnit :
	public Vehicle
{
public:
	EnemyUnit();
	EnemyUnit(int newPhysicsIndex, int newSceneObjectIndex);
	~EnemyUnit();

	bool CheckForStuck();

	int type;
	int lastMotionTime;

	bool recoveryMode = false;
};

