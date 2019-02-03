#include <list>
#include <string>
#include "Entity.h"
#include "Camera.h"
#pragma once
class Gamestate
{
public:
	Gamestate();
	~Gamestate();

	std::string button;

	double time;
	double timeStep;
	Camera camera;
	glm::vec3 light = glm::vec3(2.f, 2.f, 0.f);

	std::string state;
	std::list<Entity> Entities;
};

