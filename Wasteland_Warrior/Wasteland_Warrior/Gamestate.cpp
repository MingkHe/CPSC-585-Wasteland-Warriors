#include "Gamestate.h"
#include <iostream>

Gamestate::Gamestate()
{
	camera.pos = glm::vec3(0.f, 15.f, 0.f);
}

Gamestate::~Gamestate()
{
}

void Gamestate::SpawnPlayer (float x, float y) {
	int physicsIndex = physics_Controller->createVehicle();
	physics_Controller->setPosition(0, glm::vec3{x, 4.0f, y});
	int sceneObjectIndex = scene->generateRectPrism(5.0, 3.0, 2.0);
	playerVehicle = PlayerUnit(physicsIndex, sceneObjectIndex);
}


void Gamestate::SpawnEnemy(int type, float x, float y) {
	//int physicsIndex = physics_Controller->createVehicle();
	//physics_Controller->setPosition(0, glm::vec3{ x, 5.0f, y });
	//int sceneObjectIndex = scene->generateRectPrism(5.0, 3.0, 2.0);
	Enemies.push_back(EnemyUnit(type, x, y));

	//Mesh/Textures?
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
	if (physicsIndex == playerVehicle.physicsIndex) {
		playerVehicle.position = newPosition;
		playerVehicle.transformationMatrix = newTransformationMatrix;

		//std::cout << "Box position:  X:" << location.x << "  Y:" << location.y << "  Z:" << location.z << std::endl; //Test statement, delete it if you want
	}

}