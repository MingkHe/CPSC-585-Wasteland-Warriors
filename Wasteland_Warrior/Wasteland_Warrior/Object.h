#pragma once
#include "Entity.h"
class Object :
	public Entity
{
public:
	Object();
	~Object();

	int type;

	Object(int newPhysicsIndex, int newSceneObjectIndex, float x, float y, float z);
};

