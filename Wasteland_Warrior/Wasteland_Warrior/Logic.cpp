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
	glm::vec3 pos;
	int enemiesLeft;

	//Restart
	if (gameState->restart) {
		gameState->wave = 0;
		gameState->restart = false;

		//reset Car
		gameState->physics_Controller->setPosition(gameState->playerVehicle.physicsIndex, glm::vec3{ 0, 2, 0 });
		gameState->playerVehicle.health = 100;

		//reset AI
		for (int i = 0; i < 5; i++) {
			gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000 * i, 10000 * i, 10000 * i });
			gameState->Enemies[i].health = 100;
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

			//move up an enemy AI
			gameState->physics_Controller->setPosition(gameState->Enemies[0].physicsIndex, glm::vec3{ 35, 5, 35 });

			gameState->wave = 1;
			waveBreak = 1;
			breakTime = 5 * 60;
		}


		//WAVE 1
		else if (gameState->wave == 1) {
			enemiesLeft = 0;
			for (int i = 0; i < 1; i++) {
				if (gameState->Enemies[i].health >= 0) {
					enemiesLeft++;
				}
			}
			gameState->enemiesLeft = enemiesLeft;

			//keep suspended
			for (int i = 1; i < 5; i++) {
				gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000 * i, 10000 * i, 10000 * i });
			}

			if (enemiesLeft == 0) {
				gameState->wave = 2;

				//spawn power ups
			}
		}
		else if (waveBreak == 1) {

			//keep suspended
			for (int i = 0; i < 5; i++) {
				gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000 * i, 10000 * i, 10000 * i });
			}

			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 2;
				breakTime = 10 * 60;

				//remove powerups

				//reset AI
				for (int i = 0; i < 5; i++) {
					gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000 * i, 10000 * i, 10000 * i });
					gameState->Enemies[i].health = 100;
				}

				//move up 3 enemy AIs
				gameState->physics_Controller->setPosition(gameState->Enemies[0].physicsIndex, glm::vec3{ 15, 5, 35 });
				gameState->physics_Controller->setPosition(gameState->Enemies[1].physicsIndex, glm::vec3{ 25, 5, 35 });
				gameState->physics_Controller->setPosition(gameState->Enemies[2].physicsIndex, glm::vec3{ 35, 5, 35 });
			}
			breakTime--;
		}


		//WAVE 2
		else if (gameState->wave == 2) {
			enemiesLeft = 0;
			for (int i = 0; i < 3; i++) {
				if (gameState->Enemies[i].health >= 0) {
					enemiesLeft++;
				}
			}
			gameState->enemiesLeft = enemiesLeft;

			//keep suspended
			for (int i = 3; i < 5; i++) {
				gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000 * i, 10000 * i, 10000 * i });
			}

			if (enemiesLeft == 0) {
				gameState->wave = -1; //End here for now

				//spawn power ups
			}
		}
		else if (waveBreak == 2) {

			//keep suspended
			for (int i = 0; i < 5; i++) {
				gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000 * i, 10000 * i, 10000 * i });
			}

			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = -1; //End here for now
				breakTime = 30 * 60;
				//remove powerups

								//reset AI
				//for (int i = 0; i < 5; i++) {
				//	gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000 * i, 10000 * i, 10000 * i });
				//	gameState->Enemies[i].health = 100;
				//}

				//move up 5 enemy AIs
				//gameState->physics_Controller->setPosition(gameState->Enemies[0].physicsIndex, glm::vec3{ -15, 5, 35 });
				//gameState->physics_Controller->setPosition(gameState->Enemies[1].physicsIndex, glm::vec3{ -15, 5, 25 });
				//gameState->physics_Controller->setPosition(gameState->Enemies[2].physicsIndex, glm::vec3{ -35, 5, 35 });
				//gameState->physics_Controller->setPosition(gameState->Enemies[3].physicsIndex, glm::vec3{ -15, 5, 15 });
				//gameState->physics_Controller->setPosition(gameState->Enemies[4].physicsIndex, glm::vec3{ -35, 5, 15 });
			}
			breakTime--;
		}
		/*
		//WAVE 3
		else if (gameState->wave == 3) {
			enemiesLeft = 0;
			for (int i = 0; i < 5; i++) {
				if (gameState->Enemies[i].health >= 0) {
					enemiesLeft++;
				}
			}
			gameState->enemiesLeft = enemiesLeft;
			if (enemiesLeft == 0) {
				gameState->wave = 6;//End here for now
				//spawn power ups
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
		*/
		//Player has beaten all five waves
		else if (gameState->wave == 6) {
			gameState->UIMode = "Win";
			gameState->ui_gameplay = false;
			gameState->restart = true;
		}
	}
}