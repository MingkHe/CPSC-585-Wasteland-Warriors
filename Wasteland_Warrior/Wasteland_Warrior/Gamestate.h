#include <list>
#include <string>

#include <sys/timeb.h>
#include <stdio.h>
#include <errno.h>

#include "Entity.h"
#include "Camera.h"
#include "Scene.h"
#include "PlayerUnit.h"
#include "EnemyUnit.h"
#include "PowerUp.h"
#include "Object.h"
#include "Physics_Controller.h"

#pragma once

# define M_PI           3.14159265358979323846  /* pi */

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

	bool mouseRight;

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
	GLFWwindow *window;
	int loadingPercentage;
	//----------------------UI Buffer End-----------------------------------

	bool controller;

	//Joysticks
	float leftStickX;
	float leftStickY;
	float rightStickX;
	float rightStickY;

	//Triggers
	float leftTrigger;
	float rightTrigger;

	//Camera
	Camera camera = Camera(this);
	float cameraAngle;
	int view;
	int skyboxIndex;
	int mapGroundPhysicsIndex;

	//Time
	int time;
	int timeStep;

	int gstest = 5;

	//Graphics
	glm::vec3 light = glm::vec3(0.0f, 100.0f, 0.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float lightAttenuation = 0.000000002f;
	float lightAmbientCoefficient = 0.00f;

	glm::vec3 materialSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float materialShininess = 90000;

	unsigned char shading_model = 0;
	float radar_view = 1.f / 40.f;//this needs to be the inverse of the view distance

	glm::vec3 cubeLocation = glm::vec3{ 0.0f, 0.0f, 0.0f};
	Scene *scene;

	std::string UIMode;

	const int numOfStaticObjectInstances = 9;
	int staticObjMeshTextureIndices[9];
	const char* staticObjMeshList[9] = {
		"Objects/SkyBox/skySphere.obj",
		"Objects/Ruined_Brick_Building/ruined building_brick.obj", 
		"Objects/Wooden_train_cars/wagon.obj", 
		"Objects/Truck/truck.obj", 
		"Objects/Building1/building_lowpoly.obj",  
		"Objects/checkpointMarker.obj", 
		"Objects/canyonWalls.obj", 
		"Objects/Battle_Car_Package/OBJs/staticOilTanker.obj",
		"Objects/Tunnel/tunnel.obj"
	};

	const char* staticObjTextureList[9] = {
		 "Objects/SkyBox/skySphere_texture.jpg",
		 "Objects/Ruined_Brick_Building/ruined_building_brick.jpg",
		 "Objects/Wooden_train_cars/wagon_tex3.png",
		 "Objects/Truck/truck_tex1.png",
		 "Objects/Building1/building_lowpoly_texture.jpg",
		 "Textures/blueSmoke.jpg",
		 "Textures/canyonWallTexture2.png",
		 "Objects/Battle_Car_Package/tex/Oil Tank.jpg",
		 "Objects/Tunnel/tunnelWall.jpg"
	 };

	const int numOfDynamicObjectInstances = 6;
	int dynamicObjMeshTextureIndices[6];
	const char* dynamicObjMeshList[6] = {
	"Objects/Realistic_Box_Model/box_realistic.obj",
	"Objects/Realistic_Box_Model/box_realistic.obj",
	"Objects/Realistic_Box_Model/box_realistic.obj",
	"Objects/Realistic_Box_Model/box_realistic.obj",
	"Objects/Realistic_Box_Model/box_realistic.obj",
	"Objects/checkpointMarker.obj"};

	const char* dynamicObjTextureList[6] = {
	//"Objects/Realistic_Box_Model/box_texture_color_red.png", 
	"Objects/Realistic_Box_Model/full_health.jpg",
	"Objects/Realistic_Box_Model/large_health_boost.jpg",
	"Objects/Realistic_Box_Model/small_health_boost.png",
	"Objects/Realistic_Box_Model/armour.png",
	"Objects/Realistic_Box_Model/damage.png",
	"Textures/blueSmoke.jpg" };

	const int numOfVehicleObjectInstances = 7;
	int vehicleMeshTextureIndices[7];
	const char* vehicleMeshList[7] = { 
	"Objects/Battle_Car_Package/OBJs/playerVehicle.obj",
	"Objects/Battle_Car_Package/OBJs/enemy1_oilBarrelCar.obj",
	"Objects/Battle_Car_Package/OBJs/enemy2_truck.obj", 
	"Objects/Battle_Car_Package/OBJs/enemy3_bigBug.obj", 
	"Objects/Battle_Car_Package/OBJs/enemy4_dragster.obj", 
	"Objects/Battle_Car_Package/OBJs/enemy5_bigTruck.obj", 
	"Objects/Battle_Car_Package/OBJs/bigBadBoss.obj"};

	const char* vehicleTextureList[7] = {
	"Objects/Battle_Car_Package/tex/Bex Car 4.jpg",
	"Objects/Battle_Car_Package/tex/AX materiel 1.jpg",
	"Objects/Battle_Car_Package/tex/Battle Jip.jpg",
	"Objects/Battle_Car_Package/tex/Battle Toscar.jpg",
	"Objects/Battle_Car_Package/tex/4X Car.jpg",
	"Objects/Battle_Car_Package/tex/Small Truck.jpg",
	"Objects/Battle_Car_Package/tex/Truck Tex.jpg" };


	int mapMeshTextureIndices[1];
	const char* mapMeshList[1] = {
	"Objects/WorldMapV3Test.obj"};

	const char* mapTextureList[1] = {
	"Textures/sandTexture.jpg" };



	

	//Logic
	int wave;
	bool restart;
	int enemiesLeft;
	int checkpoints;
	int breakSeconds;
	int score;
	int scoreTime;
	std::string gameMode;

	int window_width;
	int window_height;

	int powerUpType;

	//Spawning/Despawning Entities

	//In order to make showing loading percentage in easier way, break InstantiateAllMeshes_Textures() into serveal part:
	void InstantiateAllMeshes_Textures();
	void InstantiateAllMeshes_Textures_Map();
	void InstantiateAllMeshes_Textures_Static();
	void InstantiateAllMeshes_Textures_Dynamic();
	void InstantiateAllMeshes_Textures_Vehicle();

	
	//--------------------------------------

	void SpawnMap();
	void SpawnStaticObject(int ObjectType, float x, float y, float z, float xRot, float yRot, float zRot);
	void SpawnDynamicObject(int ObjectType, float x, float y, float z, float xRot, float yRot, float zRot);
	void SpawnPlayer(float x, float y, float z, float xRot, float yRot, float zRot);
	void SpawnEnemy(int ObjectType, int AIType, float x, float y, float z, float xRot, float yRot, float zRot);
	void DespawnEnemy(Vehicle* vehicle);
	void DespawnObject(Object* object);

	void Collision(Vehicle* entity1, Vehicle* entity2, glm::vec3 impulse);
	void Collision(Vehicle* vehicle, PowerUp* powerUp);
	void Collision(Vehicle* vehicle, Object* staticObject);

	glm::mat4 getRotationMatrix(float xRot, float yRot, float zRot);
	void resetOrientation();
	void resetOrientation(int physicsIndex);

	void updateEntity(int physicsIndex, glm::vec3 newPosition, glm::mat4 newTransformationMatrix, float newSpeed);
	Object* lookupSOUsingPI(int physicsIndex);
	PowerUp* lookupPUUsingPI(int physicsIndex);
	Vehicle* lookupVUsingPI(int physicsIndex);
	int lookupGSIUsingPI(int physicsIndex);

	glm::mat4 getEntityTransformation(int sceneObjectIndex);
};