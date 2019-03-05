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
	//car crash with static object
	bool carCrashStatic_sound;
	//car collects power up
	bool carPowerUp_sound;

	//the sound for select the start button
	bool ui_enter;
	//the sound for ui menu switching between button
	bool ui_switch;
	//menu bgm
	bool ui_menu;
	//pause menu
	bool ui_pauseMenu;
	//game play
	bool ui_gameplay;
	
	//win bgm
	bool ui_win;
	//lose bgm
	bool ui_lose;

	//variable to indicate the engine volumn
	float engineAccel = 0.0f;
	//----------------------Sound Buffer End--------------------------------

	//----------------------UI Buffer Start---------------------------------
	bool powerText;
	int textTime;
	//----------------------UI Buffer End-----------------------------------

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
	glm::vec3 light = glm::vec3(0.0f, 150.0f, 0.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	double lightAttenuation = 0.0000002f;
	float lightAmbientCoefficient = 0.00f;

	glm::vec3 materialSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float materialShininess = 90000;

	unsigned char shading_model = 0;
	float radar_view = 1.f / 40.f;//this needs to be the inverse of the view distance

	glm::vec3 cubeLocation = glm::vec3{ 0.0f, 0.0f, 0.0f};
	Scene *scene;

	std::string UIMode;

	//Logic
	int wave;
	bool restart;
	int enemiesLeft;
	int breakSeconds;

	int window_width;
	int window_height;

	//Spawning/Despawning Entities

	void SpawnMap();
	void SpawnStaticObject(int ObjectType, float x, float y, float z);
	void SpawnDynamicObject(int ObjectType, float x, float y, float z);
	void SpawnPlayer(float x, float y, float z);
	void SpawnEnemy(int type, float x, float y, float z);
	void DespawnEnemy(EnemyUnit enemy);

	void Collision(Vehicle* entity1, Vehicle* entity2, glm::vec2 impulse);
	void Collision(Vehicle* vehicle, PowerUp* powerUp);
	void Collision(Vehicle* vehicle, Object* staticObject);

	void resetOrientation();
	void resetOrientation(int physicsIndex);

	void updateEntity(int physicsIndex, glm::vec3 newPosition, glm::mat4 newTransformationMatrix, float newSpeed);
	Object* lookupSOUsingPI(int physicsIndex);
	PowerUp* lookupPUUsingPI(int physicsIndex);
	Vehicle* lookupVUsingPI(int physicsIndex);
	int lookupGSIUsingPI(int physicsIndex);

	glm::mat4 getEntityTransformation(int sceneObjectIndex);
};