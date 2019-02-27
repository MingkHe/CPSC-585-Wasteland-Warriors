#pragma once
#include "Vehicle.h"
class EnemyUnit :
	public Vehicle
{
public:
	EnemyUnit();
	~EnemyUnit();

	EnemyUnit(int type, float x, float y);

	int type;
};

