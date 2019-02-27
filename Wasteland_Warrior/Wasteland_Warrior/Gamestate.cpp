#include "Gamestate.h"
#include "Vehicle.h"

Gamestate::Gamestate()
{
	camera.setGamestate(this);
	camera.pos = glm::vec3(0.f, 15.f, 0.f);

	Vehicle playerVehicle = Vehicle();
	Entities.push_back(playerVehicle);
}

Gamestate::~Gamestate()
{
}

