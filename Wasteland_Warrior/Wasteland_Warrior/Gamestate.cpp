#include "Gamestate.h"
#include "Vehicle.h"

Gamestate::Gamestate()
{
	camera.radius = 20;
	camera.pos = glm::vec3(0.f, 15.f, 0.f);
	camera.dir = glm::vec3(1.f, 0.f, 0.0f);

	Vehicle playerVehicle = Vehicle();
	Entities.push_back(playerVehicle);
}

Gamestate::~Gamestate()
{
}

