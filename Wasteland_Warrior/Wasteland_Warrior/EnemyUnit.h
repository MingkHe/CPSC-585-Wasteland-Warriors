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

	int AIType;
	int AITypeRevert;	//Used to store the original AI type so after becoming unstuck it continues the original behavior
	int ObjectType;

	int lastMotionTime;

	bool recoveryMode = false;
	bool forceRelocate = false;
	int active = 1;

	glm::vec2 destination = glm::vec2(0.0f, 0.0f);
};

