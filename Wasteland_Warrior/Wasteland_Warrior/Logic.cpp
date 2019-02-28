#include "Logic.h"
#include "EnemyUnit.h"

Logic::Logic()
{
}

Logic::~Logic()
{
}

void Logic::Update(Gamestate* gameState)
{
	if (gameState->playerVehicle.health > 0.0) {
		if (wave == 0) {
			//create 3 enemy AIs
			for (int i = 0; i < 3; i++) {
				gameState->SpawnEnemy(1, 0.0, 0.0);
			}
			wave = 1;
			waveBreak = 1;
			breakTime = 30 * 60;
		}

		//WAVE 1
		else if (wave == 1) {
			//if there are no enemy AIs left
			if (gameState->Enemies.empty()) {
				wave = 2;
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
					gameState->SpawnEnemy(1, 0.0, 0.0);
				}
			}
			breakTime--;
		}

		//WAVE 2
		else if (wave == 2) {
			//if there are no enemy AIs left
			if (gameState->Enemies.empty()) {
					wave = 3;
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
					gameState->SpawnEnemy(1, 0.0, 0.0);
				}
			}
			breakTime--;
		}

		//WAVE 3
		else if (wave == 3) {
			//if there are no enemy AIs left
			if (gameState->Enemies.empty()) {
				wave = 4;
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
					gameState->SpawnEnemy(1, 0.0, 0.0);
				}
			}
			breakTime--;
		}

		//WAVE 4
		else if (wave == 4) {
			//if there are no enemy AIs left
			if (gameState->Enemies.empty()) {
				wave = 5;
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
					gameState->SpawnEnemy(1, 0.0, 0.0);
				}
			}
			breakTime--;
		}

		//WAVE 5
		else if (wave == 5) {
		//if there are no enemy AIs left
		if (gameState->Enemies.empty()) {
			wave = 6;
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
		else if (wave == 6) {
			//Game over
		}
	}

	//Player has lost all health
	else {
		//Game over
	}
}