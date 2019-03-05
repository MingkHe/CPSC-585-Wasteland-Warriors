#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
}

Object::Object(int newPhysicsIndex, int newSceneObjectIndex, float x, float y, float z)
{
	physicsIndex = newPhysicsIndex;
	sceneObjectIndex = newSceneObjectIndex;
	this->position = glm::vec3(x, y, z);
}