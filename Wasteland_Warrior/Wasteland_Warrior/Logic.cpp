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
		gameState->physics_Controller->setPosition(gameState->playerVehicle.physicsIndex, glm::vec3{ 0, 2, 0 });
		gameState->playerVehicle.health = 100;

		//Despawn Enemies ***
		//reset enemies
		for (int i = 0; i < gameState->Enemies.size(); i++) {
			gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000 + (i * 10), 10000 + (i * 10), 10000 + (i * 10) });
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
			gameState->wave = 1;
			waveBreak = 1;
			breakTime = 1800;
			modeSelection(gameState);
		}


		//WAVE 1
		else if (gameState->wave == 1) {
			enemiesLeft = checkEnemyHealth(gameState);
			if (enemiesLeft == 0) {
				spawnPowerUps(gameState);
				gameState->wave = 2;
			}
		}

		//BREAK 1
		else if (waveBreak == 1) {
			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 2;
				breakTime = 1800;
				modeSelection(gameState);
			}
			breakTime--;
		}


		//WAVE 2
		else if (gameState->wave == 2) {
			enemiesLeft = checkEnemyHealth(gameState);
			if (enemiesLeft == 0) {
				spawnPowerUps(gameState);
				gameState->wave = 3;
			}
		}

		//BREAK 2
		else if (waveBreak == 2) {
			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 3;
				breakTime = 1800;
				modeSelection(gameState);
			}
			breakTime--;
		}
		

		//WAVE 3
		else if (gameState->wave == 3) {
			enemiesLeft = checkEnemyHealth(gameState);
			if (enemiesLeft == 0) {
				spawnPowerUps(gameState);
				gameState->wave = -1; //End here for now
			}
		}
		//BREAK 2
		else if (waveBreak == 3) {
			if (breakTime <= 0) {
				waveBreak = -1; //End here for now
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

int Logic::checkEnemyHealth(Gamestate *gameState) {
	int enemiesLeft = 0;
	int end = gameState->Enemies.size()-1;
	for (int i = 1; i <= gameState->wave; i++) {
		if (gameState->Enemies[end].health >= 0) {
			enemiesLeft++;
		}
		else {
			//Despawn Enemies *** this needs the proper despawn function ***
			gameState->physics_Controller->setPosition(gameState->Enemies[end].physicsIndex, glm::vec3{ 10000 + (i * 10), 10000 + (i * 10), 10000 + (i * 10) });
		}
		end--;
	}
	gameState->enemiesLeft = enemiesLeft;
	return enemiesLeft;
}

void Logic::spawnPowerUps(Gamestate *gameState) {

	for (int i = 0; i < gameState->wave; i++) {
		int mode = i % 4;
		switch (mode) {
		case 0: //quadrant 1
			gameState->SpawnDynamicObject(1, 25 + (i * 10), 0, 25 + (i * 10));
			break;
		case 1: //quadrant 2
			gameState->SpawnDynamicObject(1, -25 - (i * 10), 1, 25 + (i * 10));
			break;
		case 2: //quadrant 3
			gameState->SpawnDynamicObject(1, 25 + (i * 10), 1, -25 - (i * 10));
			break;
		case 3: //quadrant 4
			gameState->SpawnDynamicObject(1, -25 - (i * 10), 1, -25 - (i * 10));
			break;
		}
	}
}

void Logic::modeSelection(Gamestate *gameState) {

	//Random mode selection
	srand(time(NULL));
	int mode = rand() % 3 + 1; //3 modes for now
	mode = 1; // reset to mode 1 for now

	switch (mode) {
	case 1:
		mode1(gameState);
		break;
	case 2:
		mode2(gameState);
		break;
	case 3:
		mode3(gameState);
		break;
	}
}

//Default mode 
//Spawn as many enemies as the wave number
void Logic::mode1(Gamestate *gameState) {
	for (int i = 0; i < gameState->wave; i++) {
		int mode = i % 4;
		switch (mode) {
		case 0: //quadrant 1
			gameState->SpawnEnemy(0, 35 + (i * 10), 5, 35 + (i * 10));
			gameState->Enemies[i + 1].health = 1; //50;
			break;
		case 1: //quadrant 2
			gameState->SpawnEnemy(0, -35 - (i * 10), 5, 35 + (i * 10));
			gameState->Enemies[i + 1].health = 1; //50;
			break;
		case 2: //quadrant 3
			gameState->SpawnEnemy(0, 35 + (i * 10), 5, -35 - (i * 10));
			gameState->Enemies[i + 1].health = 1; //50;
			break;
		case 3: //quadrant 4
			gameState->SpawnEnemy(0, -35 - (i * 10), 5, -35 - (i * 10));
			gameState->Enemies[i + 1].health = 1; //50;
			break;
		}
	}
}

void Logic::mode2(Gamestate *gameState) {

}

void Logic::mode3(Gamestate *gameState) {

}