#include "EnemyUnit.h"

EnemyUnit::EnemyUnit()
{
}

EnemyUnit::EnemyUnit(int newPhysicsIndex, int newSceneObjectIndex) {
	this->health = 100;
	physicsIndex = newPhysicsIndex;
	sceneObjectIndex = newSceneObjectIndex;
}

EnemyUnit::~EnemyUnit()
{
}