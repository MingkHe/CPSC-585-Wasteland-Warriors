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
	//std::cout << gameState->playerVehicle.position.x << " " <<  gameState->playerVehicle.position.y << " " << gameState->playerVehicle.position.z << std::endl;

	//Randomization
	srand((unsigned int)time(NULL));

	//Stabilize powerups and checkpoints
	for (int i = 0; i < (int)gameState->PowerUps.size(); i++) {
		gameState->resetOrientation(gameState->PowerUps[i].physicsIndex);
	}

	//Restart
	if (gameState->restart) {

		//Reset Car
		gameState->physics_Controller->setPosition(gameState->playerVehicle.physicsIndex, glm::vec3{ 100, 2, -170 });
		gameState->playerVehicle.health = 100;
		gameState->playerVehicle.maxhealth = 100;
		gameState->playerVehicle.damageMultiplier = 1;
		gameState->playerVehicle.armor = 0.0;
		gameState->view = 0;

		//Reset Enemies
		for (int i = 0; i < (int)gameState->Enemies.size(); i++) {
			gameState->DespawnEnemy(&gameState->Enemies[i]);
		}
		gameState->Enemies.clear();

		//Reset Checkpoints
		for (int i = 0; i < (int)gameState->PowerUps.size(); i++) {
			if (gameState->PowerUps[i].type == 0) {
				gameState->DespawnPowerUp(&gameState->PowerUps[i]);
			}
		}

		//Reset powerups
		for (int i = 0; i < (int)gameState->PowerUps.size(); i++) {
			gameState->DespawnPowerUp(&gameState->PowerUps[i]);
			gameState->PowerUps.erase(gameState->PowerUps.begin() + i);
		}
		gameState->SpawnDynamicObject(1, 53, 1, -35, 0, 0, 0);
		gameState->SpawnDynamicObject(1, -100, 5.25, -100, 0, 0, 0);
		gameState->SpawnDynamicObject(1, 100, -5.25, 100, 0, 0, 0);
		gameState->SpawnDynamicObject(1, -100, 1, 100, 0, 0, 0);

		//Reset Game
		gameState->restart = false;
		gameState->wave = 1;
		score = 0;
		gameState->enemyscore = 0;
		waveBreak = 1;
		breakTime = 600;
		modeSelection(gameState);
	}

	//Player has lost all health
	if ((gameState->playerVehicle.health <= 0)) {
		gameState->UIMode = "Lose";
		gameState->ui_gameplay = false;
		gameState->restart = true;
		gameState->score = ((score / 25) * 10) + gameState->enemyscore;
		gameState->scoreTime = score / 60;
	}
	else {

		/*--- WAVE 1 ---*/
		if (gameState->wave == 1) {
			if (waveFinished(gameState)) {
				spawnPowerUps(gameState);
				gameState->wave = 2;
			}
		}

		//BREAK 1
		else if (waveBreak == 1) {
			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 2;
				breakTime = 600;
				modeSelection(gameState);
			}
			breakTime--;
		}


		/*--- WAVE 2 ---*/
		else if (gameState->wave == 2) {
			if (waveFinished(gameState)) {
				spawnPowerUps(gameState);
				gameState->wave = 3;
			}
		}

		//BREAK 2
		else if (waveBreak == 2) {
			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 3;
				breakTime = 600;
				modeSelection(gameState);
			}
			breakTime--;
		}


		/*--- WAVE 3 ---*/
		else if (gameState->wave == 3) {
			if (waveFinished(gameState)) {
				spawnPowerUps(gameState);
				gameState->wave = 4;
			}
		}

		//BREAK 3
		else if (waveBreak == 3) {
			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 4;
				breakTime = 600;
				modeSelection(gameState);
			}
			breakTime--;
		}

		/*--- WAVE 4 ---*/
		else if (gameState->wave == 4) {
			if (waveFinished(gameState)) {
				spawnPowerUps(gameState);
				gameState->wave = 5;
			}
		}

		//BREAK 4
		else if (waveBreak == 4) {
			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = 5;
				breakTime = 600;
				modeSelection(gameState);
			}
			breakTime--;
		}

		/*--- WAVE 5 ---*/
		else if (gameState->wave == 5) {
			if (waveFinished(gameState)) {
				spawnPowerUps(gameState);
				gameState->wave = -1;
			}
		}

		//BREAK 5
		//Endgame
		else if (waveBreak == 5) {
			gameState->breakSeconds = breakTime / 60;
			if (breakTime <= 0) {
				waveBreak = -1;
			}
			breakTime--;
		}

		//Player has beaten all 5 waves
		else if (gameState->wave == -1 || waveBreak == -1) {
			gameState->UIMode = "Win";
			gameState->ui_gameplay = false;
			gameState->restart = true;
			gameState->score = (int)(2160010000 / (score + 1)) + gameState->enemyscore;
			gameState->scoreTime = (int)(score / 60);
		}

	}

	score++;
}

int Logic::checkEnemyHealth(Gamestate *gameState) {
	int enemiesLeft = 0;
	for (int i = 0; i < (int)gameState->Enemies.size(); i++) {
		if (gameState->Enemies[i].health >= 0) {
			enemiesLeft++;
		}
		else {
			gameState->DespawnEnemy(&gameState->Enemies[i]);
			gameState->Enemies.erase(gameState->Enemies.begin()+i);
		}
	}
	gameState->enemiesLeft = enemiesLeft;
	return enemiesLeft;
}

bool Logic::waveFinished(Gamestate *gameState) {

	if (gameState->gameMode == "Checkpoint") {
		checkEnemyHealth(gameState);
		int checkpoints = 0;
		for (int i = 0; i < (int)gameState->PowerUps.size(); i++) {
			if (gameState->PowerUps[i].type == 0) {
				if (gameState->PowerUps[i].active) {
					checkpoints++;
				}
				else {
					gameState->DespawnPowerUp(&gameState->PowerUps[i]);
				}
			}
		}
		gameState->checkpointsLeft = checkpoints;
		if (checkpoints == 0) {
			for (int i = 0; i < (int)gameState->PowerUps.size(); i++) {
				if (gameState->PowerUps[i].type == 0) {
					gameState->DespawnPowerUp(&gameState->PowerUps[i]);
				}
			}
			for (int i = 0; i < (int)gameState->Enemies.size(); i++) {
				gameState->DespawnEnemy(&gameState->Enemies[i]);
			}
			gameState->Enemies.clear();
			gameState->enemiesLeft = 0;
			return true;
		}
	}
	else if (gameState->gameMode == "Payload") {
		checkEnemyHealth(gameState);
	}
	else if (gameState->gameMode == "Head Hunter") {
		int huntedEnemiesLeft = 0;
		for (int i = 0; i < (int)gameState->Enemies.size(); i++) {
			if (gameState->Enemies[i].headhunter && gameState->Enemies[i].health <= 0) {
				gameState->Enemies[i].headhunter = false;
				gameState->DespawnEnemy(&gameState->Enemies[i]);
				gameState->Enemies.erase(gameState->Enemies.begin() + i);
			}
			else if (gameState->Enemies[i].headhunter){
				huntedEnemiesLeft++;
			}
		}
		checkEnemyHealth(gameState);
		if (huntedEnemiesLeft == 0){
			for (int i = 0; i < (int)gameState->Enemies.size(); i++) {
				gameState->DespawnEnemy(&gameState->Enemies[i]);
			}
			gameState->Enemies.clear();
			gameState->enemiesLeft = 0;
			return true;
		}
	}
	else if (gameState->gameMode == "Survival" || gameState->gameMode == "Boss Battle") {
		if (checkEnemyHealth(gameState) == 0) {
			return true;
		}
	}
	return false;
}

void Logic::spawnPowerUps(Gamestate *gameState) {

	for (int i = 0; i < gameState->wave + 3; i++) {
		int mode = i % 4;
		switch (mode) {
		case 0: //Spawn Point 1
			gameState->SpawnDynamicObject(rand() % 5 + 1, 25.f + (i * 10.f), 0.f, 25.f + (i * 10.f), 0, 0, 0);
			break;
		case 1: //Spawn Point 2
			gameState->SpawnDynamicObject(rand() % 5 + 1, -25.f - (i * 10.f), 0.f, 25.f + (i * 10.f), 0, 0, 0);
			break;
		case 2: //Spawn Point 3
			gameState->SpawnDynamicObject(rand() % 5 + 1, 25.f + (i * 10.f), 0.f, -25.f - (i * 10.f), 0, 0, 0);
			break;
		case 3: //Spawn Point 4
			gameState->SpawnDynamicObject(rand() % 5 + 1, -25.f - (i * 10.f), 0.f, -25.f - (i * 10.f), 0, 0, 0);
			break;
		}
	}
}

void Logic::modeSelection(Gamestate *gameState) {
	switch (rand() % 4 + 1) {
	case 1:
		survival(gameState);
		gameState->gameMode = "Survival";
		break;
	case 2:
		headHunter(gameState);
		gameState->gameMode = "Head Hunter";
		break;
	case 3:
		bossBattle(gameState);
		gameState->gameMode = "Boss Battle";
		break;
	case 4:
		checkpoint(gameState);
		gameState->gameMode = "Checkpoint";
		break;
	case 5:
		payload(gameState);
		gameState->gameMode = "Payload";
		break;
	}
}

//Survival
void Logic::survival(Gamestate *gameState) {
	for (int i = 0; i < gameState->wave + 1; i++) {
		switch (i % 6) {
		case 0: //Spawn Point 1
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 150.f + (i * -5.f), -2.f, -85.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 1: //Spawn Point 2
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 145.f + (i * -5.f), 21.f, 155.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 2: //Spawn Point 3
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -90.f + (i * 5.f), 21.f, 155.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 3: //Spawn Point 4
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -137.f + (i * 5.f), 9.f, 10.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 4: //Spawn Point 5
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -137.f + (i * 5.f), 9.f, -30.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 5: //Spawn Point 6
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -100.f + (i * 5.f), -2.f, -132.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		}
	}
}

//Checkpoint
void Logic::checkpoint(Gamestate *gameState) {
	for (int i = 0; i < gameState->wave + 1; i++) {
		switch (i % 4) {
		case 0:
			gameState->SpawnDynamicObject(0, 45.f + (i * 10.f), 1.f, 45.f + (i * 10.f), 0, 0, 0);
			break;
		case 1:
			gameState->SpawnDynamicObject(0, -45.f - (i * 10.f), 0.f, 45.f + (i * 10.f), 0, 0, 0);
			break;
		case 2:
			gameState->SpawnDynamicObject(0, 45.f + (i * 10.f), 0.f, -45.f - (i * 10.f), 0, 0, 0);
			break;
		case 3:
			gameState->SpawnDynamicObject(0, -45.f - (i * 10.f), 0.f, -45.f - (i * 10.f), 0, 0, 0);
			break;
		}
	}

	//Spawn Enemies
	for (int i = 0; i < gameState->wave + 1; i++) {
		switch (i % 6) {
		case 0: //Spawn Point 1
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 150.f + (i * -5.f), -2.f, -85.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 1: //Spawn Point 2
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 145.f + (i * -5.f), 21.f, 155.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 2: //Spawn Point 3
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -90.f + (i * 5.f), 21.f, 155.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 3: //Spawn Point 4
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -137.f + (i * 5.f), 9.f, 10.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 4: //Spawn Point 5
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -137.f + (i * 5.f), 9.f, -30.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 5: //Spawn Point 6
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -100.f + (i * 5.f), -2.f, -132.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		}
	}
}

//Payload
void Logic::payload(Gamestate *gameState) {
	for (int i = 0; i < gameState->wave; i++) {
		switch (i % 4) {
		case 0:
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 35.f + (i * 10.f), 5.f, 35.f + (i * 10.f), 0, 0, 0);
			gameState->SpawnDynamicObject(0, 45.f + (i * 10.f), 0.f, 45.f + (i * 10.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 1:
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -35.f - (i * 10.f), 5.f, 35.f + (i * 10.f), 0, 0, 0);
			gameState->SpawnDynamicObject(0, -45.f - (i * 10.f), 1.f, 45.f + (i * 10.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 2:
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 35.f + (i * 10.f), 5.f, -35.f - (i * 10.f), 0, 0, 0);
			gameState->SpawnDynamicObject(0, 45.f + (i * 10.f), 1.f, -45.f - (i * 10.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 3:
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -35.f - (i * 10.f), 5.f, -35.f - (i * 10.f), 0, 0, 0);
			gameState->SpawnDynamicObject(0, -45.f - (i * 10.f), 1.f, -45.f - (i * 10.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		}
	}
}

//Head hunter
void Logic::headHunter(Gamestate *gameState) {
	for (int i = 0; i < gameState->wave + 1; i++) {
		switch (i % 6) {
		case 0: //Spawn Point 1
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 150.f + (i * -5.f), -2.f, -85.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 1: //Spawn Point 2
			gameState->SpawnEnemy(rand() % 4 + 1, 1, 145.f + (i * -5.f), 21.f, 155.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			gameState->Enemies[i].headhunter = true;
			break;
		case 2: //Spawn Point 3
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -90.f + (i * 5.f), 21.f, 155.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 3: //Spawn Point 4
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -137.f + (i * 5.f), 9.f, 10.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 4: //Spawn Point 5
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -137.f + (i * 5.f), 9.f, -30.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		case 5: //Spawn Point 6
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -100.f + (i * 5.f), -2.f, -132.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies[i].health = 25.f;
			gameState->Enemies[i].maxhealth = 25.f;
			break;
		}
	}
}

//Boss battle
void Logic::bossBattle(Gamestate *gameState) {
		switch (gameState->wave) {
		case 1:
			gameState->SpawnEnemy(5, 0, -137.f, 9.f, 10.f, 0, 0, 0);
			gameState->Enemies[0].health = 50.f;
			gameState->Enemies[0].maxhealth = 50.f;
			break;
		case 2:
			gameState->SpawnEnemy(5, 0, -137.f, 9.f, 10.f, 0, 0, 0);
			gameState->Enemies[0].health = 75.f;
			gameState->Enemies[0].maxhealth = 75.f;
			break;
		case 3:
			gameState->SpawnEnemy(5, 0, -137.f, 9.f, 10.f, 0, 0, 0);
			gameState->Enemies[0].health = 100.f;
			gameState->Enemies[0].maxhealth = 100.f;
			break;
		case 4:
			gameState->SpawnEnemy(5, 0, -137.f, 9.f, 10.f, 0, 0, 0);
			gameState->Enemies[0].health = 150.f;
			gameState->Enemies[0].maxhealth = 150.f;
			break;
		case 5:
			gameState->SpawnEnemy(5, 0, -137.f, 9.f, 10.f, 0, 0, 0);
			gameState->Enemies[0].health = 200.f;
			gameState->Enemies[0].maxhealth = 200.f;
			break;
		}
}
