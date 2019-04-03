#include "Weapon_Controller.h"

Weapon_Controller::Weapon_Controller()
{
}

Weapon_Controller::~Weapon_Controller()
{
}

void Weapon_Controller::update(Gamestate* gameState)
{
	if (gameState->button == "A") {
		gameState->shoot();
	}
	
}
