#include <list>
#include <string>
#include <chrono>
#include "Entity.h"
#include "Camera.h"
#include "Scene.h"
#include "PlayerUnit.h"
#include "EnemyUnit.h"
#include "PowerUp.h"
#include "Object.h"
#include "Physics_Controller.h"

#pragma once

class Gamestate
{
public:
	Gamestate();
	~Gamestate();

	Physics_Controller* physics_Controller;

	//Entities
	PlayerUnit playerVehicle;
	Entity map = Entity();
	std::vector<EnemyUnit> Enemies;
	std::vector<PowerUp> PowerUps;
	std::vector<Object> StaticObjects;
	std::vector<Object> DynamicObjects;

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
	std::chrono::time_point<std::chrono::system_clock> time;
	double timeStep;

	int gstest = 5;

	//Graphics
	Camera camera = Camera(this);
	glm::vec3 light = glm::vec3(0.f, 6.f, 0.f);
	unsigned char shading_model = 0;

	glm::vec3 cubeLocation = glm::vec3{ 0.0f, 0.0f, 0.0f};
	Scene *scene;

	std::string UIMode;

	//Spawning/Despawning Entities
	void SpawnPlayer(float x, float y);
	void SpawnEnemy(int type, float x, float y);
	void DespawnEnemy(EnemyUnit enemy);
	void SpawnPowerUp(int type, float x, float y);
	void DespawnPowerUp(PowerUp powerUp);
	void SpawnObject(int type, float x, float y);
	void DespawnObject(Object object);

	void Collision(Entity entity1, Entity entity2, float speed1, float speed2);

	void updateEntity(int physicsIndex, glm::vec3 newPosition, glm::mat4 newTransformationMatrix);
};