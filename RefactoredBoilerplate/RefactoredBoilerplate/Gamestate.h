#include <list>
#include "Entity.h"
#pragma once
class Gamestate
{
public:
	Gamestate();
	~Gamestate();

	std::list<Entity> Entities;
};

