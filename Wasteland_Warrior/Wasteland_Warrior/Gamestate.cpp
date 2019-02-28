#include "Gamestate.h"
#include "PlayerUnit.h"
#include "EnemyUnit.h"

Gamestate::Gamestate()
{
	camera.pos = glm::vec3(0.f, 15.f, 0.f);

	Vehicle playerVehicle = Vehicle();
}

Gamestate::~Gamestate()
{
}

void Gamestate::SpawnEnemy(int type, float x, float y) {

	Enemies.push_back(EnemyUnit(type, x, y));
}

void Gamestate::DespawnEnemy(EnemyUnit enemy) {

	Enemies.remove(enemy);
}