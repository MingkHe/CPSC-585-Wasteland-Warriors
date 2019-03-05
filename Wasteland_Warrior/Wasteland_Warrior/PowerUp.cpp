#include "PowerUp.h"

PowerUp::PowerUp()
{
}

PowerUp::~PowerUp()
{
}

PowerUp::PowerUp(int type, int newPhysicsIndex, int newSceneObjectIndex,  float x, float y, float z)
{
	physicsIndex = newPhysicsIndex;
	sceneObjectIndex = newSceneObjectIndex;
	this->type = type;
	this->position = glm::vec3(x, y, z);
}