#include "Logic.h"
#include "EnemyUnit.h"
#include "Physics_Controller.h"
#include <iostream>

Logic::Logic()
{
}

Logic::~Logic()
{
}

void Logic::Update(Gamestate *gameState)
{
	if (gameState->playerVehicle.health > 0.0) {

		std::cout << gameState->Enemies[0].position.z << std::endl;
		gameState->Enemies[0].position.z = 0;

		//Initialize
		if (gameState->wave == 0) {

			//move up 3 enemy AIs
			
			//gameState->Enemies[0].position.z = 2;
			//gameState->Enemies[1].position.z = 2;
			//gameState->Enemies[2].position.z = 2;

			gameState->wave = 1;
			waveBreak = 1;
			breakTime = 30 * 60;
		}

		//WAVE 1
		else if (gameState->wave == 1) {
			//if there are no enemy AIs left
			gameState->Enemies[0].position.z = gameState->Enemies[0].position.z - 1;
			if (gameState->Enemies.empty()) {
				gameState->wave = 2;
				//spawn power ups
			}
			else {
				//Despawn/Explode enemies that are dead 
				for (EnemyUnit const& enemy : gameState->Enemies) {
					if (enemy.health <= 0.0) {
						gameState->DespawnEnemy(enemy);
					}
				}
			}
		}
		else if (waveBreak == 1) {
			if (breakTime <= 0) {
				waveBreak = 2;
				breakTime = 30 * 60;
				//remove powerups
				//create 5 enemy AIs
				for (int i = 0; i < 5; i++) {
					//gameState->SpawnEnemy(1, 0.0, 0.0);
				}
			}
			breakTime--;
		}

		//WAVE 2
		else if (gameState->wave == 2) {
			//if there are no enemy AIs left
			if (gameState->Enemies.empty()) {
				gameState->wave = 3;
					//spawn power ups
			}
			else {
				//Despawn/Explode enemies that are dead 
				for (EnemyUnit const& enemy : gameState->Enemies) {
					if (enemy.health <= 0.0) {
						gameState->DespawnEnemy(enemy);
					}
				}
			}
		}
		else if (waveBreak == 2) {
			if (breakTime <= 0) {
				waveBreak = 3;
				breakTime = 30 * 60;
				//remove powerups
				//create 10 enemy AIs
				for (int i = 0; i < 10; i++) {
					//gameState->SpawnEnemy(1, 0.0, 0.0);
				}
			}
			breakTime--;
		}

		//WAVE 3
		else if (gameState->wave == 3) {
			//if there are no enemy AIs left
			if (gameState->Enemies.empty()) {
				gameState->wave = 4;
				//spawn power ups
			}
			else {
				//Despawn/Explode enemies that are dead 
				for (EnemyUnit const& enemy : gameState->Enemies) {
					if (enemy.health <= 0.0) {
						gameState->DespawnEnemy(enemy);
					}
				}
			}
		}
		else if (waveBreak == 3) {
			if (breakTime <= 0) {
				waveBreak = 4;
				breakTime = 30 * 60;
				//remove powerups
				//create 15 enemy AIs
				for (int i = 0; i < 15; i++) {
					//gameState->SpawnEnemy(1, 0.0, 0.0);
				}
			}
			breakTime--;
		}

		//WAVE 4
		else if (gameState->wave == 4) {
			//if there are no enemy AIs left
			if (gameState->Enemies.empty()) {
				gameState->wave = 5;
				//spawn power ups
			}
			else {
				//Despawn/Explode enemies that are dead 
				for (EnemyUnit const& enemy : gameState->Enemies) {
					if (enemy.health <= 0.0) {
						gameState->DespawnEnemy(enemy);
					}
				}
			}
		}
		else if (waveBreak == 4) {
			if (breakTime <= 0) {
				waveBreak = 5;
				breakTime = 30 * 60;
				//remove powerups
				//create 20 enemy AIs
				for (int i = 0; i < 20; i++) {
					//gameState->SpawnEnemy(1, 0.0, 0.0);
				}
			}
			breakTime--;
		}

		//WAVE 5
		else if (gameState->wave == 5) {
		//if there are no enemy AIs left
		if (gameState->Enemies.empty()) {
			gameState->wave = 6;
			//spawn power ups
		}
		else {
			//Despawn/Explode enemies that are dead 
			for (EnemyUnit const& enemy : gameState->Enemies) {
				if (enemy.health <= 0.0) {
					gameState->DespawnEnemy(enemy);
				}
			}
		}
		}

		//Player has beaten all five waves
		else if (gameState->wave == 6) {
		gameState->wave = 0;
			gameState->UIMode = "Win";
		}
	}

	//Player has lost all health
	else {
	gameState->wave = 0;
		gameState->UIMode = "lose";
	}

}