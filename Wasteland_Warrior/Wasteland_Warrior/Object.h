#pragma once
#include "Entity.h"
class Object :
	public Entity
{
public:
	Object();
	~Object();

	int type;

	Object(int type, float x, float y);
};

