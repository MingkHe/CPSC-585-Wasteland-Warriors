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
	this->score = 0;
}

Logic::~Logic()
{
}

void Logic::Update(Gamestate *gameState)
{
	//Restart
	if (gameState->restart) {
		gameState->restart = false;

		//Reset Car
		gameState->physics_Controller->setPosition(gameState->playerVehicle.physicsIndex, glm::vec3{ 0, 3, 0 });
		gameState->playerVehicle.health = 100;

		//Reset Enemies
		for (int i = 0; i < gameState->Enemies.size(); i++) {
			gameState->physics_Controller->setPosition(gameState->Enemies[i].physicsIndex, glm::vec3{ 10000 + (i * 10), 10000 + (i * 10), 10000 + (i * 10) });
		}

		//Reset Game
		gameState->wave = 1;
		score = 0;
		waveBreak = 1;
		breakTime = 1800;
		modeSelection(gameState);
	}

	//Player has lost all health
	if ((gameState->playerVehicle.health <= 0)) {
		gameState->UIMode = "Lose";
		gameState->ui_gameplay = false;
		gameState->restart = true;
		gameState->score = 2160010000 / (score + 1);
		gameState->scoreTime = score / 60;
	} 
	else {

		/*--- WAVE 1 ---*/
		if (gameState->wave == 1) {
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


		/*--- WAVE 2 ---*/
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
		

		/*--- WAVE 3 ---*/
		else if (gameState->wave == 3) {
			enemiesLeft = checkEnemyHealth(gameState);
			if (enemiesLeft == 0) {
				spawnPowerUps(gameState);
				gameState->wave = 4;
			}
		}

		//BREAK 3
		else if (waveBreak == 3) {
			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 4;
				breakTime = 1800;
				modeSelection(gameState);
			}
			breakTime--;
		}
		
		/*--- WAVE 4 ---*/
		else if (gameState->wave == 4) {
			enemiesLeft = checkEnemyHealth(gameState);
			if (enemiesLeft == 0) {
				spawnPowerUps(gameState);
				gameState->wave = 5;
			}
		}

		//BREAK 4
		else if (waveBreak == 4) {
			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 5;
				breakTime = 1800;
				modeSelection(gameState);
			}
			breakTime--;
		}

		/*--- WAVE 5 ---*/
		else if (gameState->wave == 5) {
			enemiesLeft = checkEnemyHealth(gameState);
			if (enemiesLeft == 0) {
				spawnPowerUps(gameState);
				gameState->wave = -1;
			}
		}

		//BREAK 5
		else if (waveBreak == 5) {
			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = -1;
				breakTime = 1800;
				modeSelection(gameState);
			}
			breakTime--;
		}

		//Player has beaten all 5 waves
		else if (gameState->wave == -1) {
			gameState->UIMode = "Win";
			gameState->ui_gameplay = false;
			gameState->restart = true;
			gameState->score = 2160010000 / (score + 1);
			gameState->scoreTime = score / 60;
		}

		score++;
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
			//Despawn Enemies
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
		case 0: //Spawn Point 1
			gameState->SpawnDynamicObject(1, 25 + (i * 10), 0, 25 + (i * 10));
			break;
		case 1: //Spawn Point 2
			gameState->SpawnDynamicObject(1, -25 - (i * 10), 1, 25 + (i * 10));
			break;
		case 2: //Spawn Point 3
			gameState->SpawnDynamicObject(1, 25 + (i * 10), 1, -25 - (i * 10));
			break;
		case 3: //Spawn Point 4
			gameState->SpawnDynamicObject(1, -25 - (i * 10), 1, -25 - (i * 10));
			break;
		}
	}
}

void Logic::modeSelection(Gamestate *gameState) {

	srand(time(NULL));
	switch (rand() % 5 + 1) {
	case 1:
		survival(gameState);
		gameState->gameMode = "Survival";
		break;
	case 2:
		checkpoint(gameState);
		gameState->gameMode = "Checkpoint";
		break;
	case 3:
		payload(gameState);
		gameState->gameMode = "Payload";
		break;
	case 4:
		headHunter(gameState);
		gameState->gameMode = "Head Hunter";
		break;
	case 5:
		bossBattle(gameState);
		gameState->gameMode = "Boss Battle";
		break;
	}
}

//Survival
void Logic::survival(Gamestate *gameState) {
	for (int i = 0; i < gameState->wave; i++) {
		switch (i % 4) {
		case 0: //Spawn Point 1
			gameState->SpawnEnemy(0, 35 + (i * 10), 5, 35 + (i * 10));
			gameState->Enemies[i].health = 50; //50;
			break;
		case 1: //Spawn Point 2
			gameState->SpawnEnemy(0, -35 - (i * 10), 5, 35 + (i * 10));
			gameState->Enemies[i].health = 50; //50;
			break;
		case 2: //Spawn Point 3
			gameState->SpawnEnemy(0, 35 + (i * 10), 5, -35 - (i * 10));
			gameState->Enemies[i].health = 50; //50;

			break;
		case 3: //Spawn Point 4
			gameState->SpawnEnemy(0, -35 - (i * 10), 5, -35 - (i * 10));
			gameState->Enemies[i].health = 50; //50;

			break;
		}
	}
}

//Checkpoint
void Logic::checkpoint(Gamestate *gameState) {
	for (int i = 0; i < gameState->wave; i++) {
		switch (i % 4) {
		case 0:
			gameState->SpawnEnemy(0, 35 + (i * 10), 5, 35 + (i * 10));
			gameState->SpawnDynamicObject(1, 45 + (i * 10), 0, 45 + (i * 10));
			gameState->Enemies[i + 1].health = 50;
			break;
		case 1:
			gameState->SpawnEnemy(0, -35 - (i * 10), 5, 35 + (i * 10));
			gameState->SpawnDynamicObject(1, -45 - (i * 10), 1, 45 + (i * 10));
			gameState->Enemies[i + 1].health = 50;
			break;
		case 2:
			gameState->SpawnEnemy(0, 35 + (i * 10), 5, -35 - (i * 10));
			gameState->SpawnDynamicObject(1, 45 + (i * 10), 1, -45 - (i * 10));
			gameState->Enemies[i + 1].health = 50;
			break;
		case 3:
			gameState->SpawnEnemy(0, -35 - (i * 10), 5, -35 - (i * 10));
			gameState->SpawnDynamicObject(1, -45 - (i * 10), 1, -45 - (i * 10));
			gameState->Enemies[i + 1].health = 50;
			break;
		}
	}
}

//Payload
void Logic::payload(Gamestate *gameState) {
	for (int i = 0; i < gameState->wave; i++) {
		switch (i % 4) {
		case 0:
			gameState->SpawnEnemy(0, 35 + (i * 10), 5, 35 + (i * 10));
			gameState->Enemies[i + 1].health = 50;
			break;
		case 1:
			gameState->SpawnEnemy(0, -35 - (i * 10), 5, 35 + (i * 10));
			gameState->Enemies[i + 1].health = 50;
			break;
		case 2:
			gameState->SpawnEnemy(0, 35 + (i * 10), 5, -35 - (i * 10));
			gameState->Enemies[i + 1].health = 50;
			break;
		case 3:
			gameState->SpawnEnemy(0, -35 - (i * 10), 5, -35 - (i * 10));
			gameState->Enemies[i + 1].health = 50;
			break;
		}
	}
}

//Head hunter
void Logic::headHunter(Gamestate *gameState) {
	for (int i = 0; i < gameState->wave; i++) {
		switch (i % 4) {
		case 0:
			gameState->SpawnEnemy(0, 35 + (i * 10), 5, 35 + (i * 10));
			gameState->Enemies[i].health = 50;
			break;
		case 1:
			gameState->SpawnEnemy(0, -35 - (i * 10), 5, 35 + (i * 10));
			gameState->Enemies[i].health = 50;
			break;
		case 2:
			gameState->SpawnEnemy(0, 35 + (i * 10), 5, -35 - (i * 10));
			gameState->Enemies[i].health = 50;
			break;
		case 3:
			gameState->SpawnEnemy(0, -35 - (i * 10), 5, -35 - (i * 10));
			gameState->Enemies[i].health = 50;
			break;
		}
	}
}

//Boss battle
void Logic::bossBattle(Gamestate *gameState) {
		switch (gameState->wave) {
		case 1:
			gameState->SpawnEnemy(0, 35, 5, 35);
			gameState->Enemies[1].health = 10;
			break;
		case 2:
			gameState->SpawnEnemy(0, 35, 5, 35);
			gameState->Enemies[1].health = 50;
			break;
		case 3:
			gameState->SpawnEnemy(0, 35, 5, 35);
			gameState->Enemies[1].health = 75;
			break;
		case 4:
			gameState->SpawnEnemy(0, 35, 5, 35);
			gameState->Enemies[1].health = 100;
			break;
		case 5:
			gameState->SpawnEnemy(0, 35, 5, 35);
			gameState->Enemies[1].health = 150;
			break;
		}
}