#include "Gamestate.h"
#include "PlayerUnit.h"
#include "EnemyUnit.h"

Gamestate::Gamestate()
{
	camera.setGamestate(this);
	camera.radius = 20;
	camera.pos = glm::vec3(0.f, 15.f, 0.f);
	camera.dir = glm::vec3(1.f, 0.f, 0.0f);
}

Gamestate::~Gamestate()
{
}

void Gamestate::SpawnEnemy(int type, float x, float y) {

	Enemies.push_back(EnemyUnit(type, x, y));
}