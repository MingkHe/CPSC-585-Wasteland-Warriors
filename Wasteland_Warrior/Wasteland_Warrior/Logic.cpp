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
	int enemiesLeft;

	//Restart
	if (gameState->restart) {
		gameState->wave = 0;
		gameState->restart = false;

		//reset Car
		gameState->physics_Controller->setPosition(gameState->playerVehicle.physicsIndex, glm::vec3{ 0, 0, 0 });
		gameState->playerVehicle.health = 100;

		//reset enemies
		if (gameState->Enemies.size() > 1) {
		gameState->Enemies.erase(gameState->Enemies.begin(), gameState->Enemies.begin() + gameState->Enemies.size() - 1);
		}
		for (int i = 0; i < gameState->Enemies.size(); i++) {
			gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000, 10000, 10000 });
		}
	}


	//Player has lost all health
	if ((gameState->playerVehicle.health <= 0)) {
		gameState->UIMode = "Lose";
		gameState->ui_gameplay = false;
		gameState->restart = true;
	} 
	else {


		//Initialize
		if (gameState->wave == 0) {

			gameState->SpawnEnemy(0, 35, 5, 35);
			gameState->Enemies[1].health == 75;

			gameState->wave = 1;
			waveBreak = 1;
			breakTime = 30 * 60;
		}


		//WAVE 1
		else if (gameState->wave == 1) {
			enemiesLeft = 0;
			for (int i = 1; i < 2; i++) {
				if (gameState->Enemies[i].health >= 0) {
					enemiesLeft++;
				}
				else {
					gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000, 10000, 10000 });
					gameState->Enemies.erase(gameState->Enemies.begin() + (i - 1));
				}
			}
			gameState->enemiesLeft = enemiesLeft;

			if (enemiesLeft == 0) {
				gameState->wave = 2;

				//spawn power ups
				gameState->SpawnDynamicObject(1, 25, 5, -25);
			}
		}
		else if (waveBreak == 1) {

			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 2;
				breakTime = 30 * 60;

				//spawn 3 enemy AIs
				gameState->SpawnEnemy(0, 35, 5, 35);
				gameState->Enemies[1].health == 75;
				gameState->SpawnEnemy(0, -35, 5, 35);
				gameState->Enemies[2].health == 75;
				gameState->SpawnEnemy(0, -35, 5, -35);
				gameState->Enemies[3].health == 75;
			}
			breakTime--;
		}


		//WAVE 2
		else if (gameState->wave == 2) {
			enemiesLeft = 0;
			for (int i = 1; i < 4; i++) {
				if (gameState->Enemies[i].health >= 0) {
					enemiesLeft++;
				}
				else {
					gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000, 10000, 10000 });
					gameState->Enemies.erase(gameState->Enemies.begin() + (i - 1));
				}
			}
			gameState->enemiesLeft = enemiesLeft;

			if (enemiesLeft == 0) {
				gameState->wave = 3;

				//spawn power ups
				gameState->SpawnDynamicObject(1, 15, 5, -15);
				gameState->SpawnDynamicObject(1, -15, 5, -15);
			}
		}
		else if (waveBreak == 2) {

			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 3;
				breakTime = 30 * 60;

				//spawn 5 enemy AIs
				gameState->SpawnEnemy(0, 35, 5, 35);
				gameState->Enemies[1].health == 75;
				gameState->SpawnEnemy(0, -35, 5, 35);
				gameState->Enemies[2].health == 75;
				gameState->SpawnEnemy(0, -35, 5, -35);
				gameState->Enemies[3].health == 75;
				gameState->SpawnEnemy(0, -25, 5, 45);
				gameState->Enemies[4].health == 75;
				gameState->SpawnEnemy(0, -25, 5, -45);
				gameState->Enemies[5].health == 75;
			}
			breakTime--;
		}
		
		//WAVE 3
		else if (gameState->wave == 3) {
			enemiesLeft = 0;
			for (int i = 1; i < 6; i++) {
				if (gameState->Enemies[i].health >= 0) {
					enemiesLeft++;
				}
				else {
					gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000, 10000, 10000 });
					gameState->Enemies.erase(gameState->Enemies.begin() + (i - 1));
				}
			}
			gameState->enemiesLeft = enemiesLeft;

			if (enemiesLeft == 0) {
				gameState->wave = -1; //End here for now

				//spawn power ups
			}
		}
		else if (waveBreak == 3) {
			if (breakTime <= 0) {
				waveBreak = -1;
			}
			breakTime--;
		}
		/*
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
		*/
		//Player has beaten all five waves
		else if (gameState->wave == 6) {
			gameState->UIMode = "Win";
			gameState->ui_gameplay = false;
			gameState->restart = true;
		}
	}
}