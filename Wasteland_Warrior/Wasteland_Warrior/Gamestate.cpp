#include "Gamestate.h"

Gamestate::Gamestate()
{
	camera.pos = glm::vec3(0.f, 15.f, 0.f);
}

Gamestate::~Gamestate()
{
}

void Gamestate::SpawnEnemy(int type, float x, float y) {

	Enemies.push_back(EnemyUnit(type, x, y));
}

void Gamestate::DespawnEnemy(EnemyUnit enemy) {

	//Enemies.remove(enemy);
}

void Gamestate::SpawnPowerUp(int type, float x, float y) {

	PowerUps.push_back(PowerUp(type, x, y));
}

void Gamestate::DespawnPowerUp(PowerUp powerUp) {

	//PowerUps.remove(powerUp);
}

void Gamestate::SpawnObject(int type, float x, float y) {

	if (type == 0) {
		DynamicObjects.push_back(Object(type, x, y));
	}
	else {
		StaticObjects.push_back(Object(type, x, y));
	}
}

void Gamestate::DespawnObject(Object object) {

	//Objects.remove(object);
}

void Gamestate::Collision(Entity entity1, Entity entity2, float speed1, float speed2) {
}