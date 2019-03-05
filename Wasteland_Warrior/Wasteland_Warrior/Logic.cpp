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

	for (int i = 0; i < gameState->Enemies.size(); i++) {
		std::cout << "enemy at:" << i << " health:" << gameState->Enemies[i].health << std::endl;
	}

	//Restart
	if (gameState->restart) {
		gameState->wave = 0;
		gameState->restart = false;

		//reset Car
		gameState->physics_Controller->setPosition(gameState->playerVehicle.physicsIndex, glm::vec3{ 0, 0, 0 });
		gameState->playerVehicle.health = 100;

		//reset enemies
		for (int i = 1; i < gameState->Enemies.size(); i++) {
			gameState->Enemies.erase(gameState->Enemies.begin() + (i - 1));
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
			gameState->Enemies[1].health == 1;

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
					gameState->Enemies.erase(gameState->Enemies.begin() + (i - 1));
				}
			}
			gameState->enemiesLeft = enemiesLeft;

			if (enemiesLeft == 0) {
				gameState->wave = 2;

				//spawn power ups
				//gameState->physics_Controller->setPosition(gameState->PowerUps[0].physicsIndex, glm::vec3{ 53, 0, -35 });
			}
		}
		else if (waveBreak == 1) {

			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 2;
				breakTime = 10 * 60;

				//remove powerups
				//gameState->physics_Controller->setPosition(gameState->PowerUps[0].physicsIndex, glm::vec3{ 20000 , 20000 , 20000  });

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
					gameState->Enemies.erase(gameState->Enemies.begin() + (i - 1));
				}
			}
			gameState->enemiesLeft = enemiesLeft;

			if (enemiesLeft == 0) {
				gameState->wave = -1; //End here for now

				//spawn power ups
			}
		}
		else if (waveBreak == 2) {

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