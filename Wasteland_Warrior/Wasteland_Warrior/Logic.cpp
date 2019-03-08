#include "Logic.h"
#include "EnemyUnit.h"
#include "Physics_Controller.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Logic::Logic()
{
	this->waveBreak = 0;
	this->breakTime = 0;
	this->enemiesLeft = 0;

}

Logic::~Logic()
{
}

void Logic::Update(Gamestate *gameState)
{
	//Restart
	if (gameState->restart) {
		gameState->wave = 0;
		gameState->restart = false;

		//Reset Car
		gameState->physics_Controller->setPosition(gameState->playerVehicle.physicsIndex, glm::vec3{ 0, 0, 0 });
		gameState->playerVehicle.health = 100;


		//Despawn Enemies ***
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

		//Setup
		if (gameState->wave == 0) {

			//Start wave 1
			gameState->wave = 1;
			waveBreak = 1;
			breakTime = 1800;

			modeSelection(gameState, gameState->wave);
			//gameState->SpawnEnemy(0, 35 + (0 * 10), 5, 35 + (0 * 10));
			//gameState->Enemies[1].health = 75;
		}


		//WAVE 1
		else if (gameState->wave == 1) {

			//Check Enemy health
			enemiesLeft = 0;
			for (int i = 1; i < 2; i++) {
				if (gameState->Enemies[i].health >= 0) {
					enemiesLeft++;
				}
				else {
					//Despawn Enemies ***
					gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000, 10000, 10000 });
					gameState->Enemies.erase(gameState->Enemies.begin() + (i - 1));
				}
			}
			gameState->enemiesLeft = enemiesLeft;

			//If all Enemies are dead start wave 2
			if (enemiesLeft == 0) {
				gameState->wave = 2;

				//Spawn powerups
				gameState->SpawnDynamicObject(1, 25, 0, -25);
			}
		}

		//BREAK 1
		else if (waveBreak == 1) {

			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 2;
				breakTime = 1800;

				//spawn 2 enemy AIs
				modeSelection(gameState, gameState->wave);
				//gameState->SpawnEnemy(0, 35, 5, 35);
				//gameState->Enemies[1].health = 75;
				//gameState->SpawnEnemy(0, -35, 5, 35);
				//gameState->Enemies[2].health = 75;
			}
			breakTime--;
		}


		//WAVE 2
		else if (gameState->wave == 2) {

			//Check Enemy health
			enemiesLeft = 0;
			for (int i = 1; i < 4; i++) {
				if (gameState->Enemies[i].health >= 0) {
					enemiesLeft++;
				}
				else {
					//Despawn Enemies ***
					gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000, 10000, 10000 });
					gameState->Enemies.erase(gameState->Enemies.begin() + (i - 1));
				}
			}
			gameState->enemiesLeft = enemiesLeft;

			if (enemiesLeft == 0) {
				gameState->wave = 3;

				//spawn power ups
				gameState->SpawnDynamicObject(1, 15, 0, -15);
				gameState->SpawnDynamicObject(1, -15, 0, -15);
			}
		}

		//BREAK 2
		else if (waveBreak == 2) {

			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 3;
				breakTime = 30 * 60;

				//spawn 3 enemy AIs
				modeSelection(gameState, gameState->wave);
				//gameState->SpawnEnemy(0, 35, 5, 35);
				//gameState->Enemies[1].health = 75;
				//gameState->SpawnEnemy(0, -35, 5, 35);
				//gameState->Enemies[2].health = 75;
				//gameState->SpawnEnemy(0, -35, 5, -35);
				//gameState->Enemies[3].health = 75;
			}
			breakTime--;
		}
		
		//WAVE 3
		else if (gameState->wave == 3) {

		//Check Enemy health
			enemiesLeft = 0;
			for (int i = 1; i < 6; i++) {
				if (gameState->Enemies[i].health >= 0) {
					enemiesLeft++;
				}
				else {

					//Despawn Enemies***
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
		
		//Player has beaten all 3 waves
		else if (gameState->wave == -1) {
			gameState->UIMode = "Win";
			gameState->ui_gameplay = false;
			gameState->restart = true;
		}
	}
}

void Logic::modeSelection(Gamestate *gameState, int wave) {

	//mode selection
	srand(time(NULL));
	int mode = rand() % 3 + 1; //3 modes
	mode = 1; // reset to mode 1 for now

	switch (mode) {
	case 1:
		mode1(gameState, wave);
		break;
	case 2:
		mode2(gameState, wave);
		break;
	case 3:
		mode3(gameState, wave);
		break;
	}
}

//Default mode //Spawn as many enemies as the wave number
void Logic::mode1(Gamestate *gameState, int wave) {
	for (int i = 0; i < wave; i++) {

		int mode = i % 4;
		switch (mode) {
		case 0: //quadrant 1
			gameState->SpawnEnemy(0, 35 + (i * 10), 5, 35 + (i * 10));
			gameState->Enemies[i+1].health = 75;
			break;
		case 1: //quadrant 2
			gameState->SpawnEnemy(0, -35 - (i * 10), 5, 35 + (i * 10));
			gameState->Enemies[i+1].health = 75;
			break;
		case 2: //quadrant 3
			gameState->SpawnEnemy(0, 35 + (i * 10), 5, -35 - (i * 10));
			gameState->Enemies[i+1].health = 75;
			break;
		case 3: //quadrant 4
			gameState->SpawnEnemy(0, -35 - (i * 10), 5, -35 - (i * 10));
			gameState->Enemies[i+1].health = 75;
			break;
		}
	}
}

void Logic::mode2(Gamestate *gameState, int wave) {

}

void Logic::mode3(Gamestate *gameState, int wave) {

}