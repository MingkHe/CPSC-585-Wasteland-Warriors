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

	std::vector<glm::vec2> pathfindingInputs;

	//Button input
	std::string button;

	//WASD
	bool WKey;
	bool AKey;
	bool SKey;
	bool DKey;
	bool SPACEKey;

	//----------------------Sound Buffer Begin-------------------------------

	/*
	Availabe sound effects for car:
	1. engine ignition 
	2. car running and acceleration
	3. car engine idle
	4. car braking
	5. car crashing (collision sound effect)
	6. car explosion

	If you want to use any sound effect above, follow the instruction:
	
	car engine igntion, car crashing and car explosion are one time sound effect,
	which means you just set its corresponding bool to true and it will automatically
	set back to false when the sound effct is done

	car running, car braking and car engine idle are continous sound effect,
	which means you need to set it's bool to true to play the sound effect, 
	and you need to set it's bool back to flase when it need to stop this sound effect.
	*/


	//car engine ignition sound
	bool carStart_sound;
	//car engine sound when the car is idle
	bool carIdle_sound;
	//engine acceleration sound
	bool carRunning_sound;
	//car braking sound
	bool carBrake_sound;
	//car crashing sound
	bool carCrash_sound;
	//car explosion sound
	bool carExpo_sound;

	//the sound for select the start button
	bool ui_enter;
	//the sound for ui menu switching between button
	bool ui_switch;
	//menu bgm
	bool ui_menu;
	//pause menu
	bool ui_pauseMenu;
	
	//win bgm
	bool ui_win;
	//lose bgm
	bool ui_lose;
	//----------------------Sound Buffer End-------------------------------

	//Joysticks
	float leftStickX;
	float leftStickY;
	float rightStickX;
	float rightStickY;

	//Triggers
	float leftTrigger;
	float rightTrigger;

	//camera rotation
	float cameraAngle;

	//Time
	std::chrono::time_point<std::chrono::system_clock> time;
	double timeStep;

	int gstest = 5;

	//Graphics
	Camera camera = Camera(this);
	glm::vec3 light = glm::vec3(0.0f, 1000.0f, 0.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	double lightAttenuation = 0.000001;
	float lightAmbientCoefficient = 0.05;

	glm::vec3 materialSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float materialShininess = 0.2;

	unsigned char shading_model = 0;
	float radar_view = 1.f / 40.f;//this needs to be the inverse of the view distance

	glm::vec3 cubeLocation = glm::vec3{ 0.0f, 0.0f, 0.0f};
	Scene *scene;

	std::string UIMode;

	int wave;
	bool restart;

	//Spawning/Despawning Entities

	void SpawnMap();
	void SpawnStaticObject(int ObjectType);
	void SpawnPlayer(float x, float y, float z);
	void SpawnEnemy(int type, float x, float y, float z);
	void SpawnEnemy2(float x, float y);
	void DespawnEnemy(EnemyUnit enemy);
	void SpawnPowerUp(int type, float x, float y);
	void DespawnPowerUp(PowerUp powerUp);
	void SpawnObject(int type, float x, float y);
	void DespawnObject(Object object);

	void Collision(Vehicle* entity1, Vehicle* entity2, glm::vec2 impulse);
	void Collision(Vehicle* vehicle, PowerUp* powerUp);


	void updateEntity(int physicsIndex, glm::vec3 newPosition, glm::mat4 newTransformationMatrix, float newSpeed);
	PowerUp* lookupPUUsingPI(int physicsIndex);
	Vehicle* lookupVUsingPI(int physicsIndex);
	int lookupGSIUsingPI(int physicsIndex);

	glm::mat4 getEntityTransformation(int sceneObjectIndex);
};