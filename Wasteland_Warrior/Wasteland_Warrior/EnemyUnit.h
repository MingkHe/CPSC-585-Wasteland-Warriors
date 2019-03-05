#pragma once
#include "Vehicle.h"
#include <chrono>

class EnemyUnit :
	public Vehicle
{
public:
	EnemyUnit();
	EnemyUnit(int newPhysicsIndex, int newSceneObjectIndex);
	~EnemyUnit();

	bool CheckForStuck();

	int type;
	std::chrono::time_point<std::chrono::system_clock> lastMotionTime;

	bool recoveryMode = false;
};

