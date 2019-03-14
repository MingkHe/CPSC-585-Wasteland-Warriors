#include "PlayerUnit.h"

PlayerUnit::PlayerUnit() {

}

PlayerUnit::PlayerUnit(int newPhysicsIndex, int newSceneObjectIndex)
{
	physicsIndex = newPhysicsIndex;
	sceneObjectIndex = newSceneObjectIndex;
	health = 200;

	heading = glm::vec2(-1, 0);
}


PlayerUnit::~PlayerUnit()
{
}
