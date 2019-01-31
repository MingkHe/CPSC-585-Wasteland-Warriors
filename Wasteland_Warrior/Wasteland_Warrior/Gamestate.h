#include <list>
#include "Entity.h"
#include "Camera.h"
#pragma once
class Gamestate
{
public:
	Gamestate();
	~Gamestate();

	double time;
	double timeStep;
	Camera camera;
	glm::vec3 light = glm::vec3(1.f, 2.f, 0.f);

	std::list<Entity> Entities;
};

