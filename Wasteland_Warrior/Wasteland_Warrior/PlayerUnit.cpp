#include "PlayerUnit.h"

PlayerUnit::PlayerUnit() {

}

PlayerUnit::PlayerUnit(int newPhysicsIndex, int newSceneObjectIndex)
{
	physicsIndex = newPhysicsIndex;
	sceneObjectIndex = newSceneObjectIndex;
	health = 200;
}


PlayerUnit::~PlayerUnit()
{
}
