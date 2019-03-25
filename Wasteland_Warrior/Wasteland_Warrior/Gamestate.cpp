#include "Gamestate.h"
#include "Scene.h"
#include <iostream>

Gamestate::Gamestate()
{
	timeStep = (int)((1.0 / 60.0) * 1000); //60 fps
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
	loadingPercentage = 0;

	cameraAngle = 0.0;
	view = 0;

	wave = 0;
	restart = false;
	enemiesLeft = 0;
}

Gamestate::~Gamestate()
{
}

void Gamestate::InstantiateAllMeshes_Textures() {

}

void Gamestate::SpawnMap() {
	int sceneObjectIndex = scene->loadOBJObject("Objects/WorldMapV3Test.obj", "Textures/sandTexture.jpg");
	std::cout << sceneObjectIndex  << "Hello"<< std::endl;
	int vertsSize = scene->allWorldCompObjects[0].subObjects[0].vertsPhys.size();
	PxVec3* vertsPhysArray = new PxVec3[vertsSize];
	glm::vec3 vertHolder = { -1,1,1 };
	for (int i = 0; i < vertsSize; i++) {
		vertHolder = scene->allWorldCompObjects[sceneObjectIndex].subObjects[0].vertsPhys[i];
		vertsPhysArray[i] = PxVec3(vertHolder.x, vertHolder.y, vertHolder.z);
	}

	int faceVertsSize = scene->allWorldCompObjects[sceneObjectIndex].subObjects[0].faceVertexIndices.size();
	PxU32* faceVertsPhys = new PxU32[faceVertsSize]; 
	PxU32 faceVertHolder = 0;
	int index = 0;
	for (int i = 0; i < faceVertsSize; i++) {
		faceVertHolder = scene->allWorldCompObjects[sceneObjectIndex].subObjects[0].faceVertexIndices[i];
		faceVertHolder--;
		faceVertsPhys[i] = PxU32(faceVertHolder);

	}
	mapGroundPhysicsIndex = physics_Controller->createMap(vertsPhysArray, vertsSize, faceVertsPhys, faceVertsSize/3);
}

void Gamestate::SpawnStaticObject(int ObjectType, float x, float y, float z) {
	bool objectExists = true;
	int sceneObjectIndex=0;
	if (ObjectType == 0) {
		sceneObjectIndex = scene->loadOBJObject("Objects/SkyBox/skySphere.obj", "Objects/SkyBox/skySphere_texture.jpg");
		skyboxIndex = sceneObjectIndex;
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
	else if (ObjectType == 4) {
		sceneObjectIndex = scene->loadOBJObject("Objects/Building1/building_lowpoly.obj", "Objects/Building1/building_lowpoly_texture.jpg");
	}
	else if (ObjectType == 4) {
		sceneObjectIndex = scene->loadOBJObject("Objects/Building1/building_lowpoly.obj", "Objects/Building1/building_lowpoly_texture.jpg");
	}
	else if (ObjectType == 5) {
		sceneObjectIndex = scene->loadOBJObject("Objects/checkpointMarker.obj", "Textures/blueSmoke.jpg");
	}
	else if (ObjectType == 6) {
		sceneObjectIndex = scene->loadOBJObject("Objects/canyonWalls.obj", "Textures/canyonWallTexture.png");
	}
	
	else {
		objectExists = false;
	}
	if (objectExists) {
		int vertsSize = scene->allWorldCompObjects[sceneObjectIndex].subObjects[0].vertsPhys.size();
		PxVec3* vertsPhysArray = new PxVec3[vertsSize];
		glm::vec3 vertHolder = { -1,1,1 };
		for (int i = 0; i < vertsSize; i++) {
			vertHolder = scene->allWorldCompObjects[sceneObjectIndex].subObjects[0].vertsPhys[i];
			vertsPhysArray[i] = PxVec3(vertHolder.x, vertHolder.y, vertHolder.z);
		}

		int faceVertsSize = scene->allWorldCompObjects[sceneObjectIndex].subObjects[0].faceVertexIndices.size();
		PxU32* faceVertsPhys = new PxU32[faceVertsSize];
		PxU32 faceVertHolder = 0;
		int index = 0;
		for (int i = 0; i < faceVertsSize; i++) {
			faceVertHolder = scene->allWorldCompObjects[sceneObjectIndex].subObjects[0].faceVertexIndices[i];
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
		scene->allWorldCompObjects[sceneObjectIndex].subObjects[0].transform = transformMatrix;

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
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			x, y, z, 1.f
		);
		scene->allWorldCompObjects[sceneObjectIndex].subObjects[0].transform = transformMatrix;
		PowerUp newPowerUp = PowerUp(1, physicsIndex, sceneObjectIndex, x, y, z);
		newPowerUp.gameStateIndex = PowerUps.size();
		PowerUps.push_back(newPowerUp);
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


void Gamestate::SpawnEnemy(int ObjectType, int AIType, float x, float y, float z) {
	int physicsIndex = physics_Controller->createEnemyVehicle();
	physics_Controller->setPosition(physicsIndex, glm::vec3{ x, y, z });
	int sceneObjectIndex;

	//Different Enemy Mesh/Texture Types *** Add new mesh/textures to this list ***
	switch (ObjectType) {
	case 0: 
		sceneObjectIndex = scene->loadOBJObject("Objects/BladedDragster/bourak.obj", "Objects/BladedDragster/bourak.jpg");
		break;
	case 1: 
		sceneObjectIndex = scene->loadOBJObject("Objects/BladedDragster/bourak.obj", "Objects/BladedDragster/bourak.jpg");
		break;
	case 2: 
		sceneObjectIndex = scene->loadOBJObject("Objects/BladedDragster/bourak.obj", "Objects/BladedDragster/bourak.jpg");
		break;
	case 3: 
		sceneObjectIndex = scene->loadOBJObject("Objects/BladedDragster/bourak.obj", "Objects/BladedDragster/bourak.jpg");
		break;
	case 4: 
		sceneObjectIndex = scene->loadOBJObject("Objects/BladedDragster/bourak.obj", "Objects/BladedDragster/bourak.jpg");
		break;
	}
	
	EnemyUnit enemy = EnemyUnit(physicsIndex, sceneObjectIndex);
	enemy.gameStateIndex = Enemies.size();
	enemy.AIType = AIType;
	Enemies.push_back(enemy);
	pathfindingInputs.push_back(glm::vec2{ 0.0f,0.0f });
}

void Gamestate::resetOrientation() {
	physics_Controller->resetOrientation(playerVehicle.physicsIndex);
}

void Gamestate::resetOrientation(int physicsIndex) {
	physics_Controller->resetOrientation(physicsIndex);
}


void Gamestate::DespawnEnemy(Vehicle* vehicle) { 
	vehicle->setActive(0);
	int offset = vehicle->physicsIndex;
	physics_Controller->setPosition(vehicle->physicsIndex, glm::vec3{20 * offset, -20, 0});
}

void Gamestate::Collision(Vehicle* entity1, Vehicle* entity2, glm::vec3 impulse) {
	// play sound when collision happen
	this->carCrash_sound = true;

	//Calculate "attack levels", how well the direction of the vehicle aligns with the axis of impulse
	glm::vec2 normalizedImpulse = glm::normalize(glm::vec2(impulse.x, impulse.z));
	float attackLevelThreshold = 0.9f;
	float entity1AttackLevel = glm::dot(entity1->direction, glm::vec3(normalizedImpulse.x,0, normalizedImpulse.y));
	std::cout << "Entity 1 attack level: " << entity1AttackLevel << std::endl;
	float entity2AttackLevel = glm::dot(entity2->direction, glm::vec3(normalizedImpulse.x, 0, normalizedImpulse.y));
	std::cout << "Entity 2 attack level: " << entity2AttackLevel << std::endl;

	if (entity1 == &playerVehicle)
		std::cout << "Player and ";
	
	else
		std::cout << "Enemy and ";

	if (entity2 == &playerVehicle)
		std::cout << "Player collided" << std::endl;

	else
		std::cout << "Enemy collided ";

	float totalForce = glm::length(impulse);
	std::cout << "with force: " << totalForce << std::endl;


	float damageScaling = 700;		//Smaller number means more damage
	float damage = totalForce / damageScaling;
	std::cout << "causeing: " << damage << " base damage (if less than 5, no damage dealt)" << std::endl;


	//Inflict damage
	//If both vehicles align meaning a rear end
	if ((entity1AttackLevel >= attackLevelThreshold && entity2AttackLevel >= attackLevelThreshold) ||
		(entity2AttackLevel <= -attackLevelThreshold && entity1AttackLevel <= -attackLevelThreshold) 
		&& damage > 5.0f) {

		//Person going slower takes damage
		if (entity1->speed > entity2->speed) {
			entity2->health -= damage * entity1->damageMultiplier;
		}
		else{
			entity1->health -= damage * entity2->damageMultiplier;
		}
	}

	//If collision is head on
	else if ((entity1AttackLevel >= attackLevelThreshold && entity2AttackLevel <= -attackLevelThreshold) ||
		(entity2AttackLevel >= attackLevelThreshold && entity1AttackLevel <= -attackLevelThreshold)
		&& damage > 5.0f) {

		entity1->health -= damage * entity2->damageMultiplier;
		entity2->health -= damage * entity1->damageMultiplier;
	}

	else {
		//std::cout << "Single collision" << std::endl;
		//if (abs(entity1AttackLevel) >= attackLevelThreshold && damage > 5.0f) 
		if (abs(entity1AttackLevel) >= abs(entity2AttackLevel) && damage > 5.0f)
			entity2->health -= damage * entity1->damageMultiplier;

		//if (abs(entity2AttackLevel) >= attackLevelThreshold&& damage > 5.0f) 
		if (abs(entity2AttackLevel) >= abs(entity1AttackLevel) && damage > 5.0f)
			entity1->health -= damage * entity2->damageMultiplier;
	}


	//Resolve effects of damage
	if (entity1->health <= 0)
		DespawnEnemy(entity1);

	if(entity2->health <= 0)
		DespawnEnemy(entity2);

	//Explosion sound
	if (entity1->health <= 0 || entity2->health <= 0)
		this->carExpo_sound = true;


	std::cout << "New health values: " << entity1->health << " | " << entity2->health << std::endl << std::endl;
}



void Gamestate::Collision(Vehicle* vehicle, PowerUp* powerUp) {
	std::cout << "Powerup picked up" << std::endl;		//Placeholder
	
	glm::mat4 transformMatrix = glm::mat4(
		2.f, 0.f, 0.f, 0.f,
		0.f, 2.f, 0.f, 0.f,
		0.f, 0.f, 2.f, 0.f,
		0.f, -3.0f, 0.f, 1.f
	);

	scene->allWorldCompObjects[powerUp->sceneObjectIndex].subObjects[0].transform = transformMatrix;  //Change location of graphic to out of sight
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
		playerVehicle.direction = glm::vec3{ -newDirection.x , newDirection.y, newDirection.z };
		//std::cout << "Player direction: [" << playerVehicle.direction.x << "," << playerVehicle.direction.y << "]" << std::endl; //Test statement, delete it if you want
		//std::cout << "Player position:  X:" << newPosition.x << "  Y:" << newPosition.y << "  Z:" << newPosition.z << std::endl; //Test statement, delete it if you want
	}
	

	for (int i = 0; i < (int)Enemies.size(); i++) {
		if (physicsIndex == Enemies[i].physicsIndex) {
			Enemies[i].direction = glm::vec3{ -newDirection.x , newDirection.y, newDirection.z };
			entityToUpdate = &Enemies[i];
			found = true;
		}
	}

	for (int i = 0; i < (int)PowerUps.size(); i++) {
		if (physicsIndex == PowerUps[i].physicsIndex) {
			//PowerUps[i].direction = glm::vec3{ -newDirection.x , newDirection.y, newDirection.z };
			entityToUpdate = &PowerUps[i];
			found = true;
		}
	}


	if (found) {
		entityToUpdate->acceleration = ((newSpeed - entityToUpdate->speed)/60);
		entityToUpdate->speed = newSpeed;
		entityToUpdate->position = newPosition;
		entityToUpdate->transformationMatrix = newTransformationMatrix;
	}
	else{
		//std::cout << "Gamestate failed to locate the physicsIndex, entity not updated" << std::endl;
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

