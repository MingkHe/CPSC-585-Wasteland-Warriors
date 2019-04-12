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
#include "Explosion.h"

#pragma once

# define M_PI           3.14159265358979323846  /* pi */

class Gamestate
{
public:
	Gamestate();
	~Gamestate();

	Physics_Controller* physics_Controller;

	bool fullscreen;

	//Entities
	PlayerUnit playerVehicle;
	Entity map = Entity();
	std::vector<EnemyUnit> Enemies;
	std::vector<EnemyUnit>SurvivingEnemies;
	std::vector<PowerUp> PowerUps;
	std::vector<Object> StaticObjects;
	std::vector<Object> DynamicObjects;
	std::vector<Explosion> explosions;

	std::vector<glm::vec2> pathfindingInputs;

	//Button input
	std::string button;

	//WASD
	bool WKey;
	bool AKey;
	bool SKey;
	bool DKey;
	bool Handbrake;

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

	//sound for machine gun
	bool weaponMachineGun_sound;
	//sound for reload;
	bool weaponReload_sound;
	//sound for bullet shell drop
	bool weaponShellDrop_sound;
	//sound for empty ammo
	bool weaponEmptyAmmo_sound;
	//sound for bullet hit metal
	bool weaponHit_sound;
	//sound for weapon swap
	bool weaponSwap_sound;

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

	//----------------------Weapon System-----------------------------------
	//int ammo;
	int weaponState; // 0 means no weapon 1 means machine gun

	//----------------------------------------------------------------------

	//---------------------Damage Indicator---------------------------------
	int damage;
	//----------------------------------------------------------------------

	//----------------------UI Buffer Start---------------------------------
	bool powerText;
	bool modeText;
	bool damageText;
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

	//Haptic Wheel 
	bool hapticWheel;
	bool updateHapticWheelState;
	

	//Camera
	Camera camera = Camera(this);
	float cameraAngle;
	int view;
	int skyboxIndex;
	int groundIndex;
	int mapGroundPhysicsIndex;
	int explosionMeshIndex;
	int mainRoadIndex;
	int mainRailroadIndex;

	CompositeWorldObject mainRoad;
	CompositeWorldObject mainRailroad;
	Geometry explosion;

	//Time
	int time;
	int timeStep;

	int gstest = 5;

	//Graphics
	glm::vec3 light = glm::vec3(20.0f, 100.0f, 0.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float lightAttenuation = 0.000000002f;
	float lightAmbientCoefficient = 0.3f;

	glm::vec3 materialSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float materialShininess = 90000;

	unsigned char shading_model = 0;
	float radar_view = 1.f / 300.f;//this needs to be the inverse of the view distance

	glm::vec3 cubeLocation = glm::vec3{ 0.0f, 0.0f, 0.0f};
	Scene *scene;

	std::string UIMode;

	const int numOfStaticObjectInstances = 15;
	int staticObjMeshTextureIndices[15];
	std::vector<const char*> staticObjMeshList[15] = { 
		{"Objects/SkyBox/skySphere.obj"},
		{"Objects/Ruined_Brick_Building/ruined building_brick.obj"},
		{"Objects/Wooden_train_cars/wagon.obj"},
		{"Objects/Truck/truck.obj"},
		{"Objects/Building1/building_lowpoly.obj"},
		{"Objects/checkpointMarker.obj"},
		{"Objects/canyonWalls.obj"},
		{"Objects/Battle_Car_Package/OBJs/staticOilTanker.obj"},
		{"Objects/Tunnel/tunnel.obj"},
		{"Objects/RuinedSmallHouse/Old_house.obj"},
		{"Objects/Buildings/Gas Station.obj"},
		{"Objects/explosionSphere.obj"},  //Explosion Sphere
		{"Objects/mainRoad.obj"},
		{"Objects/mainRailroadOptimized.obj"},
		{"Objects/bigRock.obj"}
	};

	std::vector<const char*> staticObjTextureList[15] = {
		 {"Objects/SkyBox/skySphere_texture.jpg"},
		 {"Objects/Ruined_Brick_Building/ruined_building_brick.jpg"},
		 {"Objects/Wooden_train_cars/wagon_tex3.png"},
		 {"Objects/Truck/truck_tex1.png"},
		 {"Objects/Building1/building_lowpoly_texture.jpg"},
		 {"Textures/blueSmoke.jpg"},
		 {"Textures/canyonWallTexture2.png"},
		 {"Objects/Battle_Car_Package/tex/Oil Tank.jpg"},
		 {"Objects/Tunnel/tunnelWall.jpg"},
		 {"Objects/RuinedSmallHouse/Old_house.png"},
		 {"Objects/Buildings/Gas Station.jpg"},
		 {"Textures/explosion_texture.png"}, //Explosion Texture
		{"Textures/atlas.png"}, //Road Texture
		{"Objects/Railroad/concreteRails.jpg", "Objects/Railroad/chapa oxidada.jpg", "Objects/Railroad/concreteRails.jpg", "Objects/Railroad/chapa oxidada.jpg"},
		{"Textures/bigRock.jpg"}
	 };

	const int numOfDynamicObjectInstances = 7;
	int dynamicObjMeshTextureIndices[7];
	std::vector<const char*> dynamicObjMeshList[7] = {
	{"Objects/Realistic_Box_Model/box_realistic.obj"},
	{"Objects/Realistic_Box_Model/box_realistic.obj"},
	{"Objects/Realistic_Box_Model/box_realistic.obj"},
	{"Objects/Realistic_Box_Model/box_realistic.obj"},
	{"Objects/Realistic_Box_Model/box_realistic.obj"},
	{"Objects/checkpointMarker.obj"},
	{"Objects/Realistic_Box_Model/box_realistic.obj"} };

	std::vector<const char*> dynamicObjTextureList[7] = {
	//"Objects/Realistic_Box_Model/box_texture_color_red.png", 
	{"Objects/Realistic_Box_Model/full_health.jpg"},
	{"Objects/Realistic_Box_Model/large_health_boost.jpg"},
	{"Objects/Realistic_Box_Model/small_health_boost.png"},
	{"Objects/Realistic_Box_Model/armour.png"},
	{"Objects/Realistic_Box_Model/damage.png"},
	{"Textures/blueSmoke.jpg" },
	{"Objects/Realistic_Box_Model/payload.png"} };

	const int numOfVehicleObjectInstances = 7;
	int vehicleMeshTextureIndices[7];
	std::vector<const char*> vehicleMeshList[7] = {
	{"Objects/Battle_Car_Package/OBJs/playerVehicle.obj"},
	{"Objects/Battle_Car_Package/OBJs/enemy1_oilBarrelCar.obj"},
	{"Objects/Battle_Car_Package/OBJs/enemy2_truck.obj"},
	{"Objects/Battle_Car_Package/OBJs/enemy3_bigBug.obj"},
	{"Objects/Battle_Car_Package/OBJs/enemy4_dragster.obj"},
	{"Objects/Battle_Car_Package/OBJs/enemy5_bigTruck.obj"},
	{"Objects/Battle_Car_Package/OBJs/bigBadBoss.obj"} };

	std::vector<const char*> vehicleTextureList[7] = {
	{"Objects/Battle_Car_Package/tex/Bex Car 4.jpg","Objects/Battle_Car_Package/tex/Apparatus.png", "Objects/Battle_Car_Package/tex/Armor.jpg", "Objects/Battle_Car_Package/tex/Attack buffer 1.jpg" },
	{"Objects/Battle_Car_Package/tex/AX materiel 1.jpg","Objects/Battle_Car_Package/tex/Apparatus.png", "Objects/Battle_Car_Package/tex/Attack buffer 1.jpg", "Objects/Battle_Car_Package/tex/oll.png"},
	{"Objects/Battle_Car_Package/tex/Battle Jip.jpg","Objects/Battle_Car_Package/tex/oll.png",  "Objects/Battle_Car_Package/tex/Appara.png", "Objects/Battle_Car_Package/tex/Appara.png",  "Objects/Battle_Car_Package/tex/Attack buffer 1.jpg"}, //Tires Not Mapping Properly
	{"Objects/Battle_Car_Package/tex/Battle Toscar.jpg","Objects/Battle_Car_Package/tex/Apparatus.png"},
	{"Objects/Battle_Car_Package/tex/4X Car.jpg","Objects/Battle_Car_Package/tex/Apparatus.png"},
	{"Objects/Battle_Car_Package/tex/Small Truck.jpg","Objects/Battle_Car_Package/tex/Apparatus.png"},
	{"Objects/Battle_Car_Package/tex/Truck Tex.jpg","Objects/Battle_Car_Package/tex/Apparatus.png" }};


	int mapMeshTextureIndices[1];
	std::vector<const char*> mapMeshList[1] = {
	{"Objects/WorldMapV5Optimized.obj"} };

	std::vector<const char*> mapTextureList[1] = {
	{"Textures/black_wall.jpg", "Objects/Tunnel/tunnelWall.jpg","Objects/Tunnel/tunnelWall.jpg","Textures/atlas.png", "Objects/Tunnel/tunnelWall.jpg",  "Objects/Tunnel/tunnelWall.jpg", "Textures/sandTexture.jpg"  } };



	

	//Logic
	int wave;
	bool restart;
	bool startup;
	bool resetCar;
	int enemiesLeft;
	int checkpointsLeft;
	int breakSeconds;
	int score;
	int enemyscore;
	int scoreTime;
	std::string gameMode;
	bool checkpointCollected;
	bool payloadCollected;

	int window_width;
	int window_height;
	int monitor_width;
	int monitor_height;

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
	void DespawnPowerUp(PowerUp* powerUp);

	void Collision(Vehicle* entity1, Vehicle* entity2, glm::vec3 impulse, bool hapticFeedback);
	void Collision(Vehicle* vehicle, PowerUp* powerUp);
	void Collision(Vehicle* vehicle, Object* staticObject, glm::vec3 impulse, bool hapticFeedback);

	glm::mat4 getRotationMatrix(float xRot, float yRot, float zRot);
	void resetOrientation();
	void resetOrientation(int physicsIndex);

	void updateEntity(int physicsIndex, glm::vec3 newPosition, glm::mat4 newTransformationMatrix, float newSpeed);
	Object* lookupSOUsingPI(int physicsIndex);
	PowerUp* lookupPUUsingPI(int physicsIndex);
	Vehicle* lookupVUsingPI(int physicsIndex);
	int lookupGSIUsingPI(int physicsIndex);
	void shoot();

	glm::mat4 getEntityTransformation(int sceneObjectIndex);
};