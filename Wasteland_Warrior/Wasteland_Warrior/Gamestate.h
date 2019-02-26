//#ifndef GAMESTATE_H
//#define GAMESTATE_H
#pragma once

#include <list>
#include <string>
#include "Entity.h"
#include "Camera.h"
#include "Scene.h"
#pragma once


class Gamestate
{
public:
	Gamestate();
	~Gamestate();

	//Button input
	std::string button;

	//WASD
	bool WKey;
	bool AKey;
	bool SKey;
	bool DKey;
	bool SPACEKey;

	//Joysticks
	float leftStickX;
	float leftStickY;
	float rightStickX;
	float rightStickY;

	//Triggers
	float leftTrigger;
	float rightTrigger;

	//Time
	std::chrono::time_point time;
	double timeStep;

	int gstest = 5;

	//Graphics
	Camera camera;
	glm::vec3 light = glm::vec3(0.f, 6.f, 0.f);
	unsigned char shading_model = 0;

	glm::vec3 cubeLocation = glm::vec3{ 0.0f, 0.0f, 0.0f};
	Scene *scene;

	std::string UIMode;
	std::list<Entity> Entities; 
};

//#endif