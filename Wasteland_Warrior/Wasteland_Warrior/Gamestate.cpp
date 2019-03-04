#include "Gamestate.h"
#include <iostream>

Gamestate::Gamestate()
{
	camera.pos = glm::vec3(0.f, 15.f, 0.f);

	carStart_sound = false;
	carIdle_sound = false;
	carRunning_sound = false;
	carBrake_sound = false;
	carCrash_sound = false;
	carExpo_sound = false;

	ui_enter = false;
	ui_switch = false;
	ui_menu = false;
	ui_pauseMenu = false;

	cameraAngle = 0.0;

	wave = 0;
}

Gamestate::~Gamestate()
{
}



void Gamestate::SpawnPlayer(float x, float y, float z) {
	int physicsIndex = physics_Controller->createPlayerVehicle();
	physics_Controller->setPosition(physicsIndex, glm::vec3{x, 2.0f, y});
	//int sceneObjectIndex = scene->generateRectPrism(2.4, 1.6, 1.2);
	//int sceneObjectIndex = scene->loadOBJObject("Objects/BladedDragster/bourak.obj","Objects/BladedDragster/bourak.jpg" );
	//int sceneObjectIndex = scene->loadOBJObject("Objects/Wooden_train_cars/wagon.obj", "Objects/Wooden_train_cars/wagon_tex3.png");
	int sceneObjectIndex = scene->loadOBJObject("Objects/BladedDragster/bourak.obj", "Objects/BladedDragster/bourak.jpg");
	playerVehicle = PlayerUnit(physicsIndex, sceneObjectIndex);
}


void Gamestate::SpawnEnemy(int type, float x, float y, float z) {
	int physicsIndex = physics_Controller->createEnemyVehicle();
	physics_Controller->setPosition(physicsIndex, glm::vec3{ x, 2.0f, y });
	int sceneObjectIndex = scene->loadOBJObject("Objects/BladedDragster/bourak.obj", "Objects/BladedDragster/bourak.jpg");
	//int sceneObjectIndex = scene->loadOBJObject("Objects/Wooden_train_cars/wagon.obj", "Objects/Wooden_train_cars/wagon_tex3.png");
	//int sceneObjectIndex = scene->generateRectPrism(2.4, 1.6, 1.2);
	EnemyUnit enemy = EnemyUnit(physicsIndex, sceneObjectIndex);
	Enemies.push_back(enemy);
	pathfindingInputs.push_back(glm::vec2{ 0.0f,0.0f });
}

void Gamestate::SpawnEnemy2(float x, float y) {
	int physicsIndex = physics_Controller->createEnemyVehicle();
	physics_Controller->setPosition(physicsIndex, glm::vec3{ x, 2.0f, y });
	int sceneObjectIndex = scene->loadOBJObject("Objects/BladedDragster/bourak.obj", "Objects/BladedDragster/bourak.jpg");
	//int sceneObjectIndex = scene->loadOBJObject("Objects/Wooden_train_cars/wagon.obj", "Objects/Realistic_Box_Model/box_texture_color.jpg");
	//int sceneObjectIndex = scene->generateRectPrism(2.4, 1.6, 1.2);
	EnemyUnit enemy = EnemyUnit(physicsIndex, sceneObjectIndex);
	Enemies.push_back(enemy);
	pathfindingInputs.push_back(glm::vec2{ 0.0f,0.0f });
}

void Gamestate::DespawnEnemy(EnemyUnit enemy) { // Needs to blow up or something cool

	//Add to entity system
	//Enemies.remove(enemy);

	//Add to physics system

	//Add to graphics system

	//Mesh/Textures?
}

void Gamestate::SpawnPowerUp(int type, float x, float y) {

	//Add to entity system
	PowerUps.push_back(PowerUp(type, x, y));

	//Add to physics system

	//Add to graphics system

	//Mesh/Textures?
}

void Gamestate::DespawnPowerUp(PowerUp powerUp) {

	//Add to entity system
	//PowerUps.remove(powerUp);

	//Add to physics system

	//Add to graphics system

	//Mesh/Textures?
}

void Gamestate::SpawnObject(int type, float x, float y) {

	//Add to entity system
	if (type == 0) {
		DynamicObjects.push_back(Object(type, x, y));
	}
	else {
		StaticObjects.push_back(Object(type, x, y));
	}

	//Add to physics system

	//Add to graphics system

	//Mesh/Textures?
}

void Gamestate::DespawnObject(Object object) {

	//Add to entity system
	//Objects.remove(object);

	//Add to physics system

	//Add to graphics system

	//Mesh/Textures?
}

void Gamestate::Collision(Vehicle* entity1, Vehicle* entity2, glm::vec2 impulse) {
	//Determin who is the attacker
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


	float damageScaling = 200;		//Smaller number means more damage


	//If both vehicles align 
	if ((entity1AttackLevel >= attackLevelThreshold && entity2AttackLevel >= attackLevelThreshold) ||
		(entity2AttackLevel <= -attackLevelThreshold && entity1AttackLevel <= -attackLevelThreshold)) {

		if (entity1->speed > entity2->speed) {
			entity2->health -= totalForce / damageScaling;
		}
		else{
			entity1->health -= totalForce / damageScaling;
		}
	}


	if (entity1AttackLevel >= attackLevelThreshold || entity1AttackLevel <= -attackLevelThreshold)
		entity2->health -= totalForce/ damageScaling;

	if (entity2AttackLevel >= attackLevelThreshold || entity1AttackLevel <= -attackLevelThreshold)
		entity1->health -= totalForce/ damageScaling;



	if (entity1->health <= 0)
		physics_Controller->setPosition(entity1->physicsIndex, glm::vec3{ 300.0f, 4.0f, 300.0f });\

	if(entity2->health <= 0)
		physics_Controller->setPosition(entity2->physicsIndex, glm::vec3{ 300.0f, 4.0f, 300.0f});


	std::cout << "New health values: " << entity1->health << " | " << entity2->health << std::endl;


}



void Gamestate::updateEntity(int physicsIndex, glm::vec3 newPosition, glm::mat4 newTransformationMatrix, float newSpeed) {
	Entity* entityToUpdate = NULL;
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
			entityToUpdate = &PowerUps[i];
			found = true;
		}
	}

	for (int i = 0; i < (int)StaticObjects.size(); i++) {
		if (physicsIndex == StaticObjects[i].physicsIndex) {
			entityToUpdate = &StaticObjects[i];
			found = true;
		}
	}

	for (int i = 0; i < (int)DynamicObjects.size(); i++) {
		if (physicsIndex == DynamicObjects[i].physicsIndex) {
			entityToUpdate = &DynamicObjects[i];
			found = true;
		}
	}

	if (entityToUpdate ==NULL){
		entityToUpdate = &Entity();	//If no entity was found, update is waisted
		std::cout << "Gamestate failed to locate the physicsIndex, entity not updated" << std::endl;
	}

	if (found) {
		entityToUpdate->speed = newSpeed;
		entityToUpdate->position = newPosition;
		entityToUpdate->transformationMatrix = newTransformationMatrix;
	}
}




//This will probably get called by Gamestate::updateEntity later
Vehicle* Gamestate::lookupVUsingPI(int physicsIndex) {
	Vehicle* vehicle = new Vehicle();
	bool found = false;
	if (physicsIndex == playerVehicle.physicsIndex) {
		found = true;
		vehicle = &playerVehicle;
	}

	for (int i = 0; i < (int)Enemies.size(); i++) {
		if (physicsIndex == Enemies[i].physicsIndex) {
			vehicle = &Enemies[i];
			found = true;
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