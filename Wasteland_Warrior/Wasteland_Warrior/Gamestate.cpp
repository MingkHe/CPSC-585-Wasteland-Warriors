#pragma comment(lib, "LogitechSteeringWheelLib.lib")
#include "LogitechSteeringWheelLib.h"

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

	weaponMachineGun_sound = false;
	weaponEmptyAmmo_sound = false;
	weaponHit_sound = false;
	weaponReload_sound = false;
	weaponShellDrop_sound = false;
	weaponSwap_sound = false;

	ammo = 1000;
	weaponState = 0;

	ui_enter = false;
	ui_switch = false;
	ui_menu = false;
	ui_pauseMenu = false;
	ui_gameplay = false;

	ui_win = false;
	ui_lose = false;

	powerText = false;
	modeText = false;
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

	//Initialize Map Meshes & Textures
	for (int i = 0; i < 1; i++) {
		mapMeshTextureIndices[i] = scene->loadOBJObjectInstance(mapMeshList[i], mapTextureList[i]);
	}
	//Initialize Static Object Meshes & Textures
	for (int i = 0; i < numOfStaticObjectInstances; i++) {
		staticObjMeshTextureIndices[i] = scene->loadOBJObjectInstance(staticObjMeshList[i], staticObjTextureList[i]);
		if (i == 11) {
			explosionMeshIndex = staticObjMeshTextureIndices[i];
		}
	}
	//Initialize Dynamic Object Meshes & Textures
	for (int i = 0; i < numOfDynamicObjectInstances; i++) {
		dynamicObjMeshTextureIndices[i] = scene->loadOBJObjectInstance(dynamicObjMeshList[i], dynamicObjTextureList[i]);
	}
	//Initialize Vehicle Meshes & Textures
	for (int i = 0; i < numOfVehicleObjectInstances; i++) {
		vehicleMeshTextureIndices[i] = scene->loadOBJObjectInstance(vehicleMeshList[i], vehicleTextureList[i]);
	}
}

void Gamestate::InstantiateAllMeshes_Textures_Map() {
	//Initialize Map Meshes & Textures
	for (int i = 0; i < 1; i++) {
		mapMeshTextureIndices[i] = scene->loadOBJObjectInstance(mapMeshList[i], mapTextureList[i]);
	}
}

void Gamestate::InstantiateAllMeshes_Textures_Static() {
	//Initialize Static Object Meshes & Textures
	for (int i = 0; i < numOfStaticObjectInstances; i++) {
		staticObjMeshTextureIndices[i] = scene->loadOBJObjectInstance(staticObjMeshList[i], staticObjTextureList[i]);
		if (i == 11) {
			explosionMeshIndex = staticObjMeshTextureIndices[i];
		}
	}
}

void Gamestate::InstantiateAllMeshes_Textures_Dynamic() {
	//Initialize Dynamic Object Meshes & Textures
	for (int i = 0; i < numOfDynamicObjectInstances; i++) {
		dynamicObjMeshTextureIndices[i] = scene->loadOBJObjectInstance(dynamicObjMeshList[i], dynamicObjTextureList[i]);
	}
}

void Gamestate::InstantiateAllMeshes_Textures_Vehicle() {
	//Initialize Vehicle Meshes & Textures
	for (int i = 0; i < numOfVehicleObjectInstances; i++) {
		vehicleMeshTextureIndices[i] = scene->loadOBJObjectInstance(vehicleMeshList[i], vehicleTextureList[i]);
	}
}

void Gamestate::SpawnMap() {
	int sceneObjectIndex = scene->loadCompObjectInstance(mapMeshTextureIndices[0]);
	groundIndex = sceneObjectIndex;
	//std::cout << groundIndex  << "Ground--------------------------"<< std::endl;
	//int sceneObjectIndex = scene->loadOBJObject(mapMeshList[0], mapTextureList[0]);
	//std::cout << sceneObjectIndex  << "Hello"<< std::endl;
	//int s = 0;//--------------------------------------------------
	int totalVertSize = 0;
	int totalFaceVertSize = 0;
	int currentVertCount = 0;
	int currentFacesCount = 0;
	for (int t = 0; t < scene->allWorldCompObjects[groundIndex].subObjectsCount; t++) {
		totalVertSize += scene->allWorldCompObjects[groundIndex].subObjects[t].vertsPhys.size();
		totalFaceVertSize += scene->allWorldCompObjects[groundIndex].subObjects[t].faceVertexIndices.size();
	}
	PxVec3* vertsPhysArray = new PxVec3[totalVertSize];
	PxU32* faceVertsPhys = new PxU32[totalFaceVertSize];
	for (int s = 0; s < scene->allWorldCompObjects[groundIndex].subObjectsCount; s++) {
	
		int vertsSize = scene->allWorldCompObjects[groundIndex].subObjects[s].vertsPhys.size();
		//totalVertSize += vertsSize;
		
		glm::vec3 vertHolder = { -1,1,1 };
		for (int i = 0; i < vertsSize; i++) {
			vertHolder = scene->allWorldCompObjects[groundIndex].subObjects[s].vertsPhys[i];
			vertsPhysArray[currentVertCount+i] = PxVec3(vertHolder.x, vertHolder.y, vertHolder.z);
		}
		currentVertCount += vertsSize;

		int faceVertsSize = scene->allWorldCompObjects[groundIndex].subObjects[s].faceVertexIndices.size();
		//totalFaceVertSize += faceVertsSize;
		PxU32 faceVertHolder = 0;
		int index = 0;
		for (int i = 0; i < faceVertsSize; i++) {
			faceVertHolder = scene->allWorldCompObjects[groundIndex].subObjects[s].faceVertexIndices[i];
			faceVertHolder--;
			faceVertsPhys[currentFacesCount+i] = PxU32(faceVertHolder);

		}
		currentFacesCount += faceVertsSize;
	}
	mapGroundPhysicsIndex = physics_Controller->createMap(vertsPhysArray, totalVertSize, faceVertsPhys, totalFaceVertSize /3);
}

void Gamestate::SpawnStaticObject(int ObjectType, float x, float y, float z, float xRot, float yRot, float zRot) {
	bool objectExists = true;
	int sceneObjectIndex=0;
	if (ObjectType == 0) {
		sceneObjectIndex = scene->loadCompObjectInstance(staticObjMeshTextureIndices[0]);
		//sceneObjectIndex = scene->loadOBJObject(staticObjMeshList[0], staticObjTextureList[0]);
		skyboxIndex = sceneObjectIndex;
	}
	else if ((ObjectType >= 1) && (ObjectType < numOfStaticObjectInstances)) {
		sceneObjectIndex = scene->loadCompObjectInstance(staticObjMeshTextureIndices[ObjectType]);
		//sceneObjectIndex = scene->loadOBJObject(staticObjMeshList[1], staticObjTextureList[1]);
	}
	else {
		objectExists = false;
	}
	if (objectExists) {
		int totalVertSize = 0;
		int totalFaceVertSize = 0;
		int currentVertCount = 0;
		int currentFacesCount = 0;
		for (int t = 0; t < scene->allWorldCompObjects[sceneObjectIndex].subObjectsCount; t++) {
			totalVertSize += scene->allWorldCompObjects[sceneObjectIndex].subObjects[t].vertsPhys.size();
			totalFaceVertSize += scene->allWorldCompObjects[sceneObjectIndex].subObjects[t].faceVertexIndices.size();
		}
		PxVec3* vertsPhysArray = new PxVec3[totalVertSize];
		PxU32* faceVertsPhys = new PxU32[totalFaceVertSize];
		//int s = 0; //-------------------------------------
		for (int s = 0; s < scene->allWorldCompObjects[sceneObjectIndex].subObjectsCount; s++) {

			int vertsSize = scene->allWorldCompObjects[sceneObjectIndex].subObjects[s].vertsPhys.size();
			glm::vec3 vertHolder = { -1,1,1 };
			for (int i = 0; i < vertsSize; i++) {
				vertHolder = scene->allWorldCompObjects[sceneObjectIndex].subObjects[s].vertsPhys[i];
				vertsPhysArray[currentVertCount+i] = PxVec3(vertHolder.x, vertHolder.y, vertHolder.z);
			}
			currentVertCount += vertsSize;

			int faceVertsSize = scene->allWorldCompObjects[sceneObjectIndex].subObjects[s].faceVertexIndices.size();
			PxU32 faceVertHolder = 0;
			for (int i = 0; i < faceVertsSize; i++) {
				faceVertHolder = scene->allWorldCompObjects[sceneObjectIndex].subObjects[s].faceVertexIndices[i];
				faceVertHolder--;
				faceVertsPhys[currentFacesCount+i] = PxU32(faceVertHolder);

			}
			currentFacesCount += faceVertsSize;
		}
		int physicsIndex = 0;
		if (ObjectType != 11) {
			physicsIndex = physics_Controller->createStaticObject(vertsPhysArray, totalVertSize, faceVertsPhys, totalFaceVertSize / 3, x, y, z);
		}
		else {
			physicsIndex = -1;
		}
		
		glm::mat4 transformMatrix = glm::mat4(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			x, y, z, 1.f
		);
		transformMatrix = transformMatrix * getRotationMatrix(xRot, yRot, zRot);
		//int t = 0; //---------------------------------------------------
		for (int t = 0; t < scene->allWorldCompObjects[sceneObjectIndex].subObjectsCount; t++) {
			scene->allWorldCompObjects[sceneObjectIndex].subObjects[t].transform = transformMatrix;
		}
		
		if (ObjectType == 5) {
			scene->allWorldCompObjects[sceneObjectIndex].transparent = .5f;
		}
		Object staticObject = Object(physicsIndex, sceneObjectIndex, x, y, z);
		staticObject.type = ObjectType;
		StaticObjects.push_back(staticObject);
	}

}

void Gamestate::SpawnDynamicObject(int ObjectType, float x, float y, float z, float xRot, float yRot, float zRot) {
	bool objectExists = true;
	int sceneObjectIndex = 0;
	PxReal density = 1;

	//CreateBoxObject
	switch (ObjectType)
	{
	case 0://Checkpoint
		sceneObjectIndex = scene->loadCompObjectInstance(dynamicObjMeshTextureIndices[5]);
		break;
	case 1://Max Health
		sceneObjectIndex = scene->loadCompObjectInstance(dynamicObjMeshTextureIndices[0]);
		break;
	case 2://Large health boost
		sceneObjectIndex = scene->loadCompObjectInstance(dynamicObjMeshTextureIndices[1]);
		break;
	case 3://Small health boost
		sceneObjectIndex = scene->loadCompObjectInstance(dynamicObjMeshTextureIndices[2]);
		break;
	case 4://Increase armor
		sceneObjectIndex = scene->loadCompObjectInstance(dynamicObjMeshTextureIndices[3]);
		break;
	case 5://Increase damage
		sceneObjectIndex = scene->loadCompObjectInstance(dynamicObjMeshTextureIndices[4]);
		break;
	case 6://Payload
		sceneObjectIndex = scene->loadCompObjectInstance(dynamicObjMeshTextureIndices[6]);
		break;
	default:
		objectExists = false;
		break;
	}

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
	//int s = 0; //-----------------------------------------
	for (int s = 0; s < scene->allWorldCompObjects[sceneObjectIndex].subObjectsCount; s++) {
		scene->allWorldCompObjects[sceneObjectIndex].subObjects[s].transform = transformMatrix;
	}
	if (ObjectType == 0) {
		scene->allWorldCompObjects[sceneObjectIndex].transparent = .5f;
	}

			PowerUp newPowerUp = PowerUp(1, physicsIndex, sceneObjectIndex, x, y, z);
			newPowerUp.gameStateIndex = PowerUps.size();
			newPowerUp.type = ObjectType;
			PowerUps.push_back(newPowerUp);
}

void Gamestate::SpawnPlayer(float x, float y, float z, float xRot, float yRot, float zRot) {
	int physicsIndex = physics_Controller->createPlayerVehicle();
	physics_Controller->setPosition(physicsIndex, glm::vec3{x, y, z});
	//int sceneObjectIndex = scene->loadOBJObject("Objects/BladedDragster/bourak.obj", "Objects/BladedDragster/bourak.jpg");
	//int sceneObjectIndex = vehicleMeshTextureIndices[0];
	int sceneObjectIndex = scene->loadCompObjectInstance(vehicleMeshTextureIndices[0]);
	//int sceneObjectIndex = scene->loadOBJObject("Objects/Battle_Car_Package/OBJs/playerVehicle.obj", "Objects/Battle_Car_Package/tex/Bex Car 4.jpg");
	playerVehicle = PlayerUnit(physicsIndex, sceneObjectIndex);
	resetOrientation(physicsIndex);
}


void Gamestate::SpawnEnemy(int ObjectType, int AIType, float x, float y, float z, float xRot, float yRot, float zRot) {
	int physicsIndex = physics_Controller->createEnemyVehicle();
	physics_Controller->setPosition(physicsIndex, glm::vec3{ x, y, z });
	int sceneObjectIndex;

	//Different Enemy Mesh/Texture Types
	switch (ObjectType) {
	case 0:
		sceneObjectIndex = scene->loadCompObjectInstance(vehicleMeshTextureIndices[1]);
		break;
	case 1:
		sceneObjectIndex = scene->loadCompObjectInstance(vehicleMeshTextureIndices[2]);
		break;
	case 2:
		sceneObjectIndex = scene->loadCompObjectInstance(vehicleMeshTextureIndices[3]);
		break;
	case 3:
		sceneObjectIndex = scene->loadCompObjectInstance(vehicleMeshTextureIndices[4]);
		break;
	case 4:
		sceneObjectIndex = scene->loadCompObjectInstance(vehicleMeshTextureIndices[5]);
		break;
	case 5:
		sceneObjectIndex = scene->loadCompObjectInstance(vehicleMeshTextureIndices[6]);
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
	enemyscore += 200;
	this->carExpo_sound = true;
	int offset = vehicle->physicsIndex;
	glm::mat4 transformMatrix = glm::mat4(
		2.f, 0.f, 0.f, 0.f,
		0.f, 2.f, 0.f, 0.f,
		0.f, 0.f, 2.f, 0.f,
		(-100 * offset), -500.f, -500.f, 1.f
	);
	//int s = 0; //-------------------------------------------------------
	for (int s = 0; s < scene->allWorldCompObjects[vehicle->sceneObjectIndex].subObjectsCount; s++) {
		scene->allWorldCompObjects[vehicle->sceneObjectIndex].subObjects[s].transform = transformMatrix;  //Change location of graphic to out of sight
	}

	vehicle->setActive(0);
	physics_Controller->setPosition(vehicle->physicsIndex, glm::vec3{-100 * offset, -500, -500});
}

void Gamestate::DespawnPowerUp(PowerUp* powerUp) {

	int offset = powerUp->physicsIndex;

	glm::mat4 transformMatrix = glm::mat4(
		2.f, 0.f, 0.f, 0.f,
		0.f, 2.f, 0.f, 0.f,
		0.f, 0.f, 2.f, 0.f,
		(-100 * offset), -500.f, -500.f, 1.f
	);
	//int s = 0; //-------------------------------------------------------
	for (int s = 0; s < scene->allWorldCompObjects[powerUp->sceneObjectIndex].subObjectsCount; s++) {
		scene->allWorldCompObjects[powerUp->sceneObjectIndex].subObjects[s].transform = transformMatrix;  //Change location of graphic to out of sight
	}
	physics_Controller->setPosition(powerUp->physicsIndex, glm::vec3{ -100 * offset, -500, -500 });
}

void Gamestate::Collision(Vehicle* entity1, Vehicle* entity2, glm::vec3 impulse, bool hapticFeedback) {
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
		(entity2AttackLevel <= -attackLevelThreshold && entity1AttackLevel <= -attackLevelThreshold)) {
		if (damage > 5.0f) {
			//Person going slower takes damage
			if (entity1->speed > entity2->speed) {
				entity2->health -= damage * entity1->damageMultiplier;
			}
			else {
				entity1->health -= damage * entity2->damageMultiplier;
			}
		}

		if (hapticFeedback && updateHapticWheelState) {
			LogiPlayFrontalCollisionForce(0, damage*8);
		}
	}

	//If collision is head on
	else if ((entity1AttackLevel >= attackLevelThreshold && entity2AttackLevel <= -attackLevelThreshold) ||
		(entity2AttackLevel >= attackLevelThreshold && entity1AttackLevel <= -attackLevelThreshold)) {
		if (damage > 5.0f) {
			entity1->health -= damage * entity2->damageMultiplier;
			entity2->health -= damage * entity1->damageMultiplier;
		}
		if (hapticFeedback && updateHapticWheelState) {
			LogiPlayFrontalCollisionForce(0, damage * 8);
		}
	}

	else {
		//std::cout << "Single collision" << std::endl;
		//if (abs(entity1AttackLevel) >= attackLevelThreshold && damage > 5.0f)
		if (abs(entity1AttackLevel) >= abs(entity2AttackLevel) && damage > 5.0f)
			entity2->health -= damage * entity1->damageMultiplier;

		//if (abs(entity2AttackLevel) >= attackLevelThreshold&& damage > 5.0f)
		if (abs(entity2AttackLevel) >= abs(entity1AttackLevel) && damage > 5.0f)
			entity1->health -= damage * entity2->damageMultiplier;

		if (hapticFeedback && updateHapticWheelState) {
			LogiPlaySideCollisionForce(0, damage * 8);
		}
	}
	entity1->health += entity1->armor;
	entity2->health += entity2->armor;

	//Resolve effects of damage
	if (entity1->health <= 0) {
		this->explosions.push_back(Explosion(entity1->position));
		DespawnEnemy(entity1);
	}

	if (entity2->health <= 0) {
		this->explosions.push_back(Explosion(entity2->position));
		DespawnEnemy(entity2);
	}

	//Explosion sound
	if (entity1->health <= 0 || entity2->health <= 0)
		this->carExpo_sound = true;


	std::cout << "New health values: " << entity1->health << " | " << entity2->health << std::endl << std::endl;
}

void Gamestate::Collision(Vehicle* vehicle, PowerUp* powerUp) {

	switch (powerUp->type)
		{
	case 0://Checkpoint
		powerUp->active = false;
		break;
	case 1://Heal to full health
		vehicle->health = vehicle->maxhealth;
		break;
	case 2://Max health
		vehicle->maxhealth = vehicle->maxhealth + 10;
	case 3://Health boost
		vehicle->health = vehicle->health + 10;
		break;
	case 4://Increase armor
		vehicle->armor += 0.1f;
		break;
	case 5://Increase damage
		vehicle->damageMultiplier += 0.1f;
		break;
	case 6://Payload
		powerUp->active = false;
		break;
	default:
		break;
		}

	powerUpType = powerUp->type;
	this->carPowerUp_sound = true;
	this->textTime = 3 * 60;
	this->powerText = true;
	DespawnPowerUp(powerUp);
}

/*void Gamestate::Collision(Vehicle* vehicle, Object* staticObject, glm::vec3 impulse, bool hapticFeedback) {
	float totalForce = glm::length(impulse);
	float damageScaling = 700;		//Smaller number means more damage
	float damage = totalForce / damageScaling;
	if (staticObject->type == 5) {
		checkpoints--;

		glm::mat4 transformMatrix = glm::mat4(
			2.f, 0.f, 0.f, 0.f,
			0.f, 2.f, 0.f, 0.f,
			0.f, 0.f, 2.f, 0.f,
			0.f, -3.0f, 0.f, 1.f
		);

		scene->allWorldCompObjects[staticObject->sceneObjectIndex].subObjects[0].transform = transformMatrix;  //Change location of graphic to out of sight
		physics_Controller->setPosition(staticObject->physicsIndex, glm::vec3{ 0, -10, 0 });     //Change location of physics to out of way

	}
	if (hapticFeedback && updateHapticWheelState) {
		LogiPlayFrontalCollisionForce(0, damage * 3);
	}
	std::cout << "You ran into a wall, nice driving :P" << std::endl;	//Placeholder

	// play sound when car crash to static object
=======*/
void Gamestate::Collision(Vehicle* vehicle, Object* staticObject, glm::vec3 impulse, bool hapticFeedback) {

	this->carCrashStatic_sound = true;
	if (hapticFeedback && updateHapticWheelState) {
		float totalForce = glm::length(impulse);
		float damageScaling = 700;		//Smaller number means more damage
		
		float damage = totalForce / damageScaling;
		LogiPlayFrontalCollisionForce(0, damage * 8);
	}
}

void Gamestate::updateEntity(int physicsIndex, glm::vec3 newPosition, glm::mat4 newTransformationMatrix, float newSpeed) {
	Entity* entityToUpdate = &Entity();
	glm::vec4 newDirection = glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f } *newTransformationMatrix;
	float playerOffSet = 1.73f;
	float enemyOffSet = 1.73f;


	glm::mat4 pureRotation = newTransformationMatrix;
	pureRotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 vehicleNormal = glm::vec4{ 0.0f, 1.0f, 0.0f, 0.0f} *pureRotation;

	bool found = false;
	if (physicsIndex == playerVehicle.physicsIndex) {
		found = true;

		entityToUpdate = &playerVehicle;
		playerVehicle.direction = glm::vec3{ -newDirection.x , newDirection.y, newDirection.z };

		newTransformationMatrix[3] = newTransformationMatrix[3] - (playerOffSet* vehicleNormal);
	}

	for (int i = 0; i < (int)Enemies.size(); i++) {
		if (physicsIndex == Enemies[i].physicsIndex) {
			Enemies[i].direction = glm::vec3{ -newDirection.x , newDirection.y, newDirection.z };
			entityToUpdate = &Enemies[i];

			newTransformationMatrix[3] = newTransformationMatrix[3] - (enemyOffSet* vehicleNormal);
			found = true;
		}
	}

	for (int i = 0; i < (int)PowerUps.size(); i++) {
		if (physicsIndex == PowerUps[i].physicsIndex) {
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

glm::mat4 Gamestate::getRotationMatrix(float xRot, float yRot, float zRot) {
	xRot = xRot * (float)(M_PI / 180);
	yRot = yRot * (float)(M_PI / 180);
	zRot = zRot * (float)(M_PI / 180);

	glm::mat4 Rx = glm::mat4{{1.0f,0.0f,0.0f,0.0f},
							{0.0f,cos(xRot),sin(xRot),0.0f},
							{0.0f,-sin(xRot),cos(xRot),0.0f},
							{0.0f,0.0f,0.0f,1.0f} };
	glm::mat4 Ry = glm::mat4{ {cos(yRot),0.0f,-sin(yRot),0.0f},
							{0.0f,1.0f,0.0f,0.0f},
							{sin(yRot),0.0f,cos(yRot),0.0f},
							{0.0f,0.0f,0.0f,1.0f} };
	glm::mat4 Rz = glm::mat4{ {cos(zRot),sin(zRot),0.0f,0.0f},
							{-sin(zRot),cos(zRot),0.0f,0.0f},
							{0.0f,0.0f,1.0f,0.0f},
							{0.0f,0.0f,0.0f,1.0f} };
	return(Rz*Ry*Rx);

}

void Gamestate::shoot() {
	glm::vec3 pos;
	//std::cout << physicsCL.rayCast(pos) << std::endl;
	//std::cout << "pos: x:" << pos.x << " y:" << pos.y << " z:" << pos.z << std::endl;
	if (physics_Controller->rayCast(pos)) {
		for (int i = 0; i < Enemies.size(); i++) {
			printf("raycast detected..\n");
			printf("test: %f\n", glm::distance(pos, Enemies[i].position));
			if (glm::distance(pos, Enemies[i].position) <= 3.0f) {
				Enemies[i].health -= 1;
				weaponHit_sound = true;
				printf("attack!!!\n");
			}
		}
	}
}
