#include "Weapon_Controller.h"

Weapon_Controller::Weapon_Controller()
{
}

Weapon_Controller::~Weapon_Controller()
{
}

void Weapon_Controller::update(Gamestate* gameState)
{
	//if (gameState->button == "F" || gameState->button == "A") {
		//swap(gameState);
	//}

	if (gameState->weaponState == 1) {
		if (gameState->button == "X" || gameState->mouseRight) {

			//if (gameState->ammo > 0) {
				gameState->shoot();
				//gameState->ammo -= 1;
				gameState->weaponMachineGun_sound = true;
			//}
			//else {
				//gameState->weaponEmptyAmmo_sound = true;
				//gameState->weaponMachineGun_sound = false;
			//}
			//gameState->weaponShellDrop_sound = true;
		}
		else {
			//gameState->weaponEmptyAmmo_sound = false;
			gameState->weaponMachineGun_sound = false;
			//gameState->weaponShellDrop_sound = false;
		}

		//if (gameState->button == "N" || gameState->button == "X") {
			//reload(gameState);
		//}
	}
	else {
		gameState->weaponMachineGun_sound = false;
	}
}

//void Weapon_Controller::reload(Gamestate* gameState) {
	//gameState->ammo += 1000;
	//gameState->weaponReload_sound = true;
//}

//void Weapon_Controller::swap(Gamestate* gameState) {
	//gameState->weaponState = (gameState->weaponState + 1) % 2;
	//gameState->weaponSwap_sound = true;
//}
