#include "EnemyUnit.h"

EnemyUnit::EnemyUnit()
{
}

EnemyUnit::~EnemyUnit()
{
}

EnemyUnit::EnemyUnit(int type, float x,float y)
{
	this->health = 100;
	this->type = type;
	this->position = glm::vec3(x,y,0.0);
}