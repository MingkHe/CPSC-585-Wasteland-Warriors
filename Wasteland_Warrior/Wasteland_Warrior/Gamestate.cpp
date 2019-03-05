#include "Gamestate.h"
#include "Scene.h"
#include <iostream>

Gamestate::Gamestate()
{
	timeStep = (1.0 / 60.0) * 1000; //60 fps
	button = "";
	UIMode = "Start";

	camera.pos = glm::vec3(0.f, 15.f, 0.f);

	carStart_sound = false;
	carIdle_sound = false;
	carRunning_sound = false;
	carBrake_sound = false;
	carCrash_sound = false;
	carExpo_sound = false;
	carCrashStatic_sound = false;
	carPowerUp_sound = false;

	ui_enter = false;
	ui_switch = false;
	ui_menu = false;
	ui_pauseMenu = false;
	ui_gameplay = false;

	ui_win = false;
	ui_lose = false;

	powerText = false;
	textTime = 0;

	cameraAngle = 0.0;

	//Logic
	wave = 0;
	restart = false;
	enemiesLeft = 0;
}

Gamestate::~Gamestate()
{
}

void Gamestate::SpawnMap() {
	int sceneObjectIndex = scene->loadOBJObject("Objects/WorldMapV2.obj", "Textures/sandTexture.jpg");
	int vertsSize = scene->objects[sceneObjectIndex].geometry[0].vertsPhys.size();
	PxVec3* vertsPhysArray = new PxVec3[vertsSize];
	glm::vec3 vertHolder = { -1,1,1 };
	for (int i = 0; i < vertsSize; i++) {
		vertHolder = scene->objects[sceneObjectIndex].geometry[0].vertsPhys[i];
		vertsPhysArray[i] = PxVec3(vertHolder.x, vertHolder.y, vertHolder.z);
	}

	int faceVertsSize = scene->objects[sceneObjectIndex].geometry[0].faceVertexIndices.size();
	PxU32* faceVertsPhys = new PxU32[faceVertsSize]; 
	PxU32 faceVertHolder = 0;
	int index = 0;
	for (int i = 0; i < faceVertsSize; i++) {
		faceVertHolder = scene->objects[sceneObjectIndex].geometry[0].faceVertexIndices[i];
		faceVertHolder--;
		faceVertsPhys[i] = PxU32(faceVertHolder);

	}
	map.physicsIndex = physics_Controller->createMap(vertsPhysArray, vertsSize, faceVertsPhys, faceVertsSize/3);
}

void Gamestate::SpawnStaticObject(int ObjectType, float x, float y, float z) {
	bool objectExists = true;
	int sceneObjectIndex=0;
	if (ObjectType == 0) {
		sceneObjectIndex = scene->loadOBJObject("Objects/SkyBox/skySphere.obj", "Objects/SkyBox/skySphere_texture.jpg");
	}
	else if (ObjectType == 1) {
		sceneObjectIndex = scene->loadOBJObject("Objects/Ruined_Brick_Building/ruined building_brick.obj", "Objects/Ruined_Brick_Building/ruined_building_brick.jpg");
	}
	else if (ObjectType == 2) {
		sceneObjectIndex = scene->loadOBJObject("Objects/Wooden_train_cars/wagon.obj", "Objects/Wooden_train_cars/wagon_tex3.png");
	}
	else if (ObjectType == 3) {
		sceneObjectIndex = scene->loadOBJObject("Objects/Truck/truck.obj", "Objects/Truck/truck_tex1.png");
	}
	else {
		objectExists = false;
	}
	if (objectExists) {
		int vertsSize = scene->objects[sceneObjectIndex].geometry[0].vertsPhys.size();
		PxVec3* vertsPhysArray = new PxVec3[vertsSize];
		glm::vec3 vertHolder = { -1,1,1 };
		for (int i = 0; i < vertsSize; i++) {
			vertHolder = scene->objects[sceneObjectIndex].geometry[0].vertsPhys[i];
			vertsPhysArray[i] = PxVec3(vertHolder.x, vertHolder.y, vertHolder.z);
		}

		int faceVertsSize = scene->objects[sceneObjectIndex].geometry[0].faceVertexIndices.size();
		PxU32* faceVertsPhys = new PxU32[faceVertsSize];
		PxU32 faceVertHolder = 0;
		int index = 0;
		for (int i = 0; i < faceVertsSize; i++) {
			faceVertHolder = scene->objects[sceneObjectIndex].geometry[0].faceVertexIndices[i];
			faceVertHolder--;
			faceVertsPhys[i] = PxU32(faceVertHolder);

		}
		int physicsIndex = physics_Controller->createStaticObject(vertsPhysArray, vertsSize, faceVertsPhys, faceVertsSize / 3, x, y, z);
		glm::mat4 transformMatrix = glm::mat4(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			x, y, z, 1.f
		);
		scene->objects[sceneObjectIndex].geometry[0].transform = transformMatrix;

		StaticObjects.push_back(Object(physicsIndex, sceneObjectIndex, x, y, z));
	}
	
}

void Gamestate::SpawnDynamicObject(int ObjectType, float x, float y, float z) {
	bool objectExists = true;
	int sceneObjectIndex = 0;
	PxReal density = 1;
	if (ObjectType == 1) {

		//CreateBoxObject
		sceneObjectIndex = scene->loadOBJObject("Objects/Realistic_Box_Model/box_realistic.obj", "Objects/Realistic_Box_Model/box_texture_color_red.png");

		density = 1;
		PxVec3 dimensions = { 2,2,2 };
		PxU32 mass = 1;
		PxVec3 objectMOI
		((dimensions.y*dimensions.y + dimensions.z*dimensions.z)*mass / 12.0f,
			(dimensions.x*dimensions.x + dimensions.z*dimensions.z)*mass / 12.0f,
			(dimensions.x*dimensions.x + dimensions.y*dimensions.y)*mass / 12.0f);
		int physicsIndex = physics_Controller->createDynamicObject(ObjectType, dimensions, objectMOI, mass, density, x, y, z);
		glm::mat4 transformMatrix = glm::mat4(
			2.f, 0.f, 0.f, 0.f,
			0.f, 2.f, 0.f, 0.f,
			0.f, 0.f, 2.f, 0.f,
			x, y+1.0f, z, 1.f
		);
		scene->objects[sceneObjectIndex].geometry[0].transform = transformMatrix;
		PowerUps.push_back(PowerUp(1, physicsIndex, sceneObjectIndex,  x, y, z));
		//DynamicObjects.push_back(Object(physicsIndex, sceneObjectIndex , x, y, z));
		
	}
	else {
		objectExists = false;
	}
}

void Gamestate::SpawnPlayer(float x, float y, float z) {
	int physicsIndex = physics_Controller->createPlayerVehicle();
	physics_Controller->setPosition(physicsIndex, glm::vec3{x, y, z});
	int sceneObjectIndex = scene->loadOBJObject("Objects/BladedDragster/bourak.obj", "Objects/BladedDragster/bourak.jpg");
	playerVehicle = PlayerUnit(physicsIndex, sceneObjectIndex);
	resetOrientation(physicsIndex);
}


void Gamestate::SpawnEnemy(int type, float x, float y, float z) {
	int physicsIndex = physics_Controller->createEnemyVehicle();
	physics_Controller->setPosition(physicsIndex, glm::vec3{ x, y, z });
	int sceneObjectIndex = scene->loadOBJObject("Objects/BladedDragster/bourak.obj", "Objects/BladedDragster/bourak.jpg");
	EnemyUnit enemy = EnemyUnit(physicsIndex, sceneObjectIndex);
	Enemies.push_back(enemy);
	pathfindingInputs.push_back(glm::vec2{ 0.0f,0.0f });
}

void Gamestate::resetOrientation() {
	physics_Controller->resetOrientation(playerVehicle.physicsIndex);
}

void Gamestate::resetOrientation(int physicsIndex) {
	physics_Controller->resetOrientation(physicsIndex);
}


void Gamestate::DespawnEnemy(EnemyUnit enemy) { // Needs to blow up or something cool


	//Add to entity system
	//Enemies.remove(enemy);

	//Add to physics system

	//Add to graphics system

	//Mesh/Textures?
}



void Gamestate::Collision(Vehicle* entity1, Vehicle* entity2, glm::vec2 impulse) {
	//Determin who is the attacker

	// play sound when collision happen
	this->carCrash_sound = true;
	//this->carCrashStatic_sound = true; // testing purpose
	//this->carPowerUp_sound = true; // testing purpose

	glm::vec2 normalizedImpulse = glm::normalize(impulse);
	float attackLevelThreshold = 0.9;
	float entity1AttackLevel = glm::dot(entity1->direction, normalizedImpulse);
	std::cout << "Entity 1 attack level: " << entity1AttackLevel << std::endl;
	float entity2AttackLevel = glm::dot(entity2->direction, normalizedImpulse);
	std::cout << "Entity 2 attack level: " << entity2AttackLevel << std::endl;

	if (entity1 == &playerVehicle)
		std::cout << "Player and ";
	
	else
		std::cout << "Enemy and ";

	if (entity2 == &playerVehicle)
		std::cout << "Player collided" << std::endl;

	else
		std::cout << "Enemy collided ";

	float totalForce = abs(impulse.x) + abs(impulse.y);
	std::cout << "with force: " << totalForce << std::endl;


	float damageScaling = 500;		//Smaller number means more damage


	float damage = totalForce / damageScaling;

	//If both vehicles align 
	if ((entity1AttackLevel >= attackLevelThreshold && entity2AttackLevel >= attackLevelThreshold) ||
		(entity2AttackLevel <= -attackLevelThreshold && entity1AttackLevel <= -attackLevelThreshold) && damage > 5.0f) {

		if (entity1->speed > entity2->speed) {
			entity2->health -= totalForce / damageScaling;
		}
		else{
			entity1->health -= totalForce / damageScaling;
		}
	}


	if (entity1AttackLevel >= attackLevelThreshold || entity1AttackLevel <= -attackLevelThreshold && damage > 5.0f)
		entity2->health -= totalForce/ damageScaling;

	if (entity2AttackLevel >= attackLevelThreshold || entity1AttackLevel <= -attackLevelThreshold && damage > 5.0f)
		entity1->health -= totalForce/ damageScaling;



	if (entity1->health <= 0)
		physics_Controller->setPosition(entity1->physicsIndex, glm::vec3{ 10050*entity1->health, 10050*entity1->health, 10050*entity1->health });

	if(entity2->health <= 0)
		physics_Controller->setPosition(entity2->physicsIndex, glm::vec3{ 10000 * entity1->health, 10000 * entity1->health, 10000 * entity1->health });

	//explosion sound
	if (entity1->health <= 0 || entity2->health <= 0)
		this->carExpo_sound = true;


	std::cout << "New health values: " << entity1->health << " | " << entity2->health << std::endl;
}



void Gamestate::Collision(Vehicle* vehicle, PowerUp* powerUp) {
	std::cout << "You feel more powerfull!" << std::endl;		//Placeholder
	
	glm::mat4 transformMatrix = glm::mat4(
		2.f, 0.f, 0.f, 0.f,
		0.f, 2.f, 0.f, 0.f,
		0.f, 0.f, 2.f, 0.f,
		0.f, -3.0f, 0.f, 1.f
	);

	scene->objects[powerUp->sceneObjectIndex].geometry[0].transform = transformMatrix;  //Change location of graphic to out of sight
	physics_Controller->setPosition(powerUp->physicsIndex, glm::vec3{ 0, -10, 0 });     //Change location of physics to out of way

	//heal the player to full hp
	printf("healing full hp!\n");
	vehicle->health = 100;

	// play sound when car collect power up
	this->carPowerUp_sound = true;
	// start counter for display the power up text
	this->textTime = 3 * 60; // borrow the code from loghic.h counting the break time
}




void Gamestate::Collision(Vehicle* vehicle, Object* staticObject) {
	std::cout << "You ran into a wall, nice driving :P" << std::endl;	//Placeholder

	// play sound when car crash to static object
	this->carCrashStatic_sound = true;
}












void Gamestate::updateEntity(int physicsIndex, glm::vec3 newPosition, glm::mat4 newTransformationMatrix, float newSpeed) {
	Entity* entityToUpdate = &Entity();
	glm::vec4 newDirection = glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f } *newTransformationMatrix;

	bool found = false;
	if (physicsIndex == playerVehicle.physicsIndex) {
		found = true;

		entityToUpdate = &playerVehicle;
		playerVehicle.direction = glm::vec2{ -newDirection.x , newDirection.z };
		//std::cout << "Player direction: [" << playerVehicle.direction.x << "," << playerVehicle.direction.y << "]" << std::endl; //Test statement, delete it if you want
		//std::cout << "Player position:  X:" << newPosition.x << "  Y:" << newPosition.y << "  Z:" << newPosition.z << std::endl; //Test statement, delete it if you want
	}
	

	for (int i = 0; i < (int)Enemies.size(); i++) {
		if (physicsIndex == Enemies[i].physicsIndex) {
			Enemies[i].direction = glm::vec2{ -newDirection.x , newDirection.z };
			entityToUpdate = &Enemies[i];
			found = true;
		}
	}

	for (int i = 0; i < (int)PowerUps.size(); i++) {
		if (physicsIndex == PowerUps[i].physicsIndex) {
			//PowerUps[i].direction = glm::vec2{ -newDirection.x , newDirection.z };
			entityToUpdate = &PowerUps[i];
			found = true;
		}
	}


	if (!found){
		std::cout << "Gamestate failed to locate the physicsIndex, entity not updated" << std::endl;
	}

	if (found) {
		entityToUpdate->acceleration = ((newSpeed - entityToUpdate->speed)/60);
		entityToUpdate->speed = newSpeed;
		entityToUpdate->position = newPosition;
		entityToUpdate->transformationMatrix = newTransformationMatrix;
	}


}



PowerUp* Gamestate::lookupPUUsingPI(int physicsIndex) {
	PowerUp* powerUp = NULL;
	for (int i = 0; i < (int)PowerUps.size(); i++) {
		if (physicsIndex == PowerUps[i].physicsIndex) {
			powerUp = &PowerUps[i];
		}
	}
	return powerUp;
}


Object* Gamestate::lookupSOUsingPI(int physicsIndex) {
	Object* object = NULL;
	for (int i = 0; i < (int)StaticObjects.size(); i++) {
		if (physicsIndex == StaticObjects[i].physicsIndex) {
			object = &StaticObjects[i];
		}
	}
	return object;
}

Vehicle* Gamestate::lookupVUsingPI(int physicsIndex) {
	Vehicle* vehicle = NULL;
	if (physicsIndex == playerVehicle.physicsIndex) {
		vehicle = &playerVehicle;
	}

	for (int i = 0; i < (int)Enemies.size(); i++) {
		if (physicsIndex == Enemies[i].physicsIndex) {
			vehicle = &Enemies[i];
		}
	}
	return vehicle;
}

int Gamestate::lookupGSIUsingPI(int physicsIndex) {
	int gameStateIndex = -1;
	for (int i = 0; i < (int)Enemies.size(); i++) {
		if (physicsIndex == Enemies[i].physicsIndex) {
			gameStateIndex = i;
		}
	}
	return gameStateIndex;
}

glm::mat4 Gamestate::getEntityTransformation(int sceneObjectIndex) {
	//sceneObjectIndex--;   //SceneObjectIndex is different than the dynamicObjectIndex. Currently 1 smaller.
	if (sceneObjectIndex == playerVehicle.sceneObjectIndex) {
		return playerVehicle.transformationMatrix;
	}

	for (int i = 0; i < (int)Enemies.size(); i++) {
		if (sceneObjectIndex == Enemies[i].sceneObjectIndex) {
			return Enemies[i].transformationMatrix;
		}
	}

	for (int i = 0; i < (int)PowerUps.size(); i++) {
		if (sceneObjectIndex == PowerUps[i].sceneObjectIndex) {
			return PowerUps[i].transformationMatrix;
		}
	}

	for (int i = 0; i < (int)StaticObjects.size(); i++) {
		if (sceneObjectIndex == StaticObjects[i].sceneObjectIndex) {
			return StaticObjects[i].transformationMatrix;
		}
	}

	for (int i = 0; i < (int)DynamicObjects.size(); i++) {
		if (sceneObjectIndex == DynamicObjects[i].sceneObjectIndex) {
			return DynamicObjects[i].transformationMatrix;
		}
	}

	//If object was not found, return identity matrix
	return (glm::mat4{ {1.0f,0.0f,0.0f,0.0f},
						{0.0f,1.0f,0.0f,0.0f},
						{0.0f,0.0f,1.0f,0.0f},
						{0.0f,0.0f,0.0f,1.0f} });
}

