#pragma once
#include "Gamestate.h"
#include <glm/glm.hpp>

class Weapon_Controller
{
public:
	Weapon_Controller();
	~Weapon_Controller();

	void aim(Gamestate* gameState);
	void shoot(Gamestate* gameState);

};