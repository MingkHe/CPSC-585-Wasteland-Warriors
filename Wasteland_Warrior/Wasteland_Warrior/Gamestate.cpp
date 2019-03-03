#include "Gamestate.h"
#include <iostream>

Gamestate::Gamestate()
{
	camera.pos = glm::vec3(0.f, 15.f, 0.f);
}

Gamestate::~Gamestate()
{
}

void Gamestate::testFunction(int test) {
	if (test == 0)
		std::cout << "----------------New trigger---------------" << std::endl;
	else 
		std::cout << "!!!!!!!!!!!!!!!!!!!TEST!!!!!!!!!!!!!!!!!!!" << std::endl;

}

void Gamestate::SpawnPlayer (float x, float y) {
	int physicsIndex = physics_Controller->createVehicle();
	physics_Controller->setPosition(physicsIndex, glm::vec3{x, 4.0f, y});
	int sceneObjectIndex = scene->generateRectPrism(2.4f, 1.6f, 1.2f);
	playerVehicle = PlayerUnit(physicsIndex, sceneObjectIndex);
}


void Gamestate::SpawnEnemy(float x, float y) {
	int physicsIndex = physics_Controller->createVehicle();
	physics_Controller->setPosition(physicsIndex, glm::vec3{ x, 5.0f, y });
	int sceneObjectIndex = scene->generateRectPrism(2.4f, 1.6f, 1.2f);
	EnemyUnit enemy = EnemyUnit(physicsIndex, sceneObjectIndex);
	Enemies.push_back(enemy);
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

void Gamestate::Collision(Entity entity1, Entity entity2, float speed1, float speed2) {
	//Health update
	//Sound effect
}



void Gamestate::updateEntity(int physicsIndex, glm::vec3 newPosition, glm::mat4 newTransformationMatrix) {
	Entity* updateEntity = NULL;

	if (physicsIndex == playerVehicle.physicsIndex) {
		updateEntity = &playerVehicle;
		//std::cout << "Box position:  X:" << newPosition.x << "  Y:" << newPosition.y << "  Z:" << newPosition.z << std::endl; //Test statement, delete it if you want
	}

	for (int i = 0; i < (int)Enemies.size(); i++) {
		if (physicsIndex == Enemies[i].physicsIndex) {
			updateEntity = &Enemies[i];
		}
	}

	for (int i = 0; i < (int)PowerUps.size(); i++) {
		if (physicsIndex == PowerUps[i].physicsIndex) {
			updateEntity = &PowerUps[i];
		}
	}

	for (int i = 0; i < (int)StaticObjects.size(); i++) {
		if (physicsIndex == StaticObjects[i].physicsIndex) {
			updateEntity = &StaticObjects[i];
		}
	}

	for (int i = 0; i < (int)DynamicObjects.size(); i++) {
		if (physicsIndex == DynamicObjects[i].physicsIndex) {
			updateEntity = &DynamicObjects[i];
		}
	}

	if (updateEntity==NULL){
		updateEntity = &Entity();	//If no entity was found, update is waisted
		std::cout << "Gamestate failed to locate the physicsIndex, entity not updated" << std::endl;
	}

	updateEntity->position = newPosition;
	updateEntity->transformationMatrix = newTransformationMatrix;
}

glm::mat4 Gamestate::getEntityTransformation(int sceneObjectIndex) {
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