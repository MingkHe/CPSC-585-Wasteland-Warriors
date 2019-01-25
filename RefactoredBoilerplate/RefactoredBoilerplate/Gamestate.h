#include <list>
#include "Entity.h"
#pragma once
class Gamestate
{
public:
	Gamestate();
	~Gamestate();

	double time;
	double timeStep;

	std::list<Entity> Entities;
};

