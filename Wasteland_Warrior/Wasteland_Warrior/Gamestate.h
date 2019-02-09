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

	//Button input
	std::string button;

	//Joysticks
	float leftStickX;
	float leftStickY;
	float rightStickX;
	float rightStickY;

	//Triggers
	float leftTrigger;
	float rightTrigger;

	//Time
	double time;
	double timeStep;

	//Graphics
	Camera camera;
	glm::vec3 light = glm::vec3(2.f, 2.f, 0.f);
	unsigned char shading_model = 0;

	std::string UIMode;
	std::list<Entity> Entities;
};

