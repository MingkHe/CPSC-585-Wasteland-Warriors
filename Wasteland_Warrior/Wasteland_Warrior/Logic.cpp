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
		//Check Enemies
		checkEnemyHealth(gameState);

		//Randomization
		srand((unsigned int)time(NULL));

		//Stabilize powerups and checkpoints
		for (int i = 0; i < (int)gameState->PowerUps.size(); i++) {
			gameState->resetOrientation(gameState->PowerUps[i].physicsIndex);
		}

		//Score
		score++;

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
			checkEnemyHealth(gameState);
			for (int i = 0; i < (int)gameState->Enemies.size(); i++) {
				EnemyUnit* enemy = &gameState->Enemies[i];
				gameState->DespawnEnemy(enemy);
			}
			gameState->Enemies.clear();

			//Reset Powerups/Checkpoints
			for (int i = 0; i < (int)gameState->PowerUps.size(); i++) {
				PowerUp* powerUp = &gameState->PowerUps[i];
				gameState->DespawnPowerUp(powerUp);
			}
			gameState->PowerUps.clear();
			spawnPowerUps(gameState);

			//Reset Game
			gameState->restart = false;
			gameState->wave = 1;
			score = 0;
			gameState->enemyscore = 0;
			waveBreak = 1;
			breakTime = 600;
			modeSelection(gameState);
			gameState->modeText = true;
			gameState->textTime = 10 * 60;
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

			//Player has beaten all 5 waves
			if (gameState->wave == 6 || waveBreak == 6) {
				gameState->UIMode = "Win";
				gameState->ui_gameplay = false;
				gameState->restart = true;
				gameState->score = (int)(2160010000 / (score + 1)) + gameState->enemyscore;
				gameState->scoreTime = (int)(score / 60);
			}
			else {

				//Wave
				if (gameState->wave == waveBreak) {
					if (waveFinished(gameState)) {
						checkEnemyHealth(gameState);
						spawnPowerUps(gameState);
						gameState->wave++; 
						
					}
				}

				//Break
				else {
					gameState->breakSeconds = breakTime / 60;
					if (breakTime <= 0) {
						checkEnemyHealth(gameState);
						waveBreak++;
						breakTime = 600;
						modeSelection(gameState);
						gameState->modeText = true;
						gameState->textTime = 10 * 60;
					}
					breakTime--;
				}
			}
		}
}

//Wave End
bool Logic::waveFinished(Gamestate *gameState) {

	checkEnemyHealth(gameState);

	if (gameState->gameMode == "Checkpoint") {
		return checkpointsRemaining(gameState);
	}
	else if (gameState->gameMode == "Head Hunter") {
		return huntedEnemiesRemaining(gameState);
	}
	else if (gameState->gameMode == "Boss Battle") {
		return bossRemaining(gameState);
	}
	else if (gameState->gameMode == "Payload") {
		if (payloadCollected(gameState) == 0) {
			if (payloadcollected == false) {
				checkpoint(gameState);
				payloadcollected = true;
			}
		}
		if (payloadcollected) {
			return checkpointsRemaining(gameState);
		}
	}
	else if (gameState->gameMode == "Survival") {
		if (checkEnemyHealth(gameState) == 0) {
			return true;
		}
	}
	return false;
}

//Enemies Remaining
int Logic::checkEnemyHealth(Gamestate *gameState) {
	int enemiesLeft = 0;
	for (int i = 0; i < (int)gameState->Enemies.size(); i++) {
		EnemyUnit* enemy = &gameState->Enemies[i];
		if (enemy->health >= 0) {
			enemiesLeft++;
		}
		else {
			if (enemy->headhunter) {
				enemy->headhunter = false;
			}
			if (enemy->boss) {
				enemy->boss = false;
			}
			gameState->DespawnEnemy(enemy);
			gameState->Enemies.erase(gameState->Enemies.begin() + i);
		}
	}
	gameState->enemiesLeft = enemiesLeft;
	return enemiesLeft;
}

//Checkpoints Remaining
bool Logic::checkpointsRemaining(Gamestate *gameState) {
	int checkpoints = 0;
	for (int i = 0; i < (int)gameState->PowerUps.size(); i++) {
		PowerUp* powerUp = &gameState->PowerUps[i];
		if (powerUp->type == 0) {
			if (powerUp->active) {
				checkpoints++;
			} 
		}
	}
	gameState->checkpointsLeft = checkpoints;
	checkEnemyHealth(gameState);
	if (checkpoints == 0) {
		for (int i = 0; i < (int)gameState->PowerUps.size(); i++) {
			PowerUp* powerUp = &gameState->PowerUps[i];
			if (powerUp->type == 0) {
				gameState->DespawnPowerUp(powerUp);
				gameState->PowerUps.erase(gameState->PowerUps.begin() + i);
			}
		}
		return true;
	}
	return false;
}

//Hunted Enemies Remaining
bool Logic::huntedEnemiesRemaining(Gamestate *gameState) {
	int huntedEnemiesLeft = 0;
	for (int i = 0; i < (int)gameState->Enemies.size(); i++) {
		EnemyUnit* enemy = &gameState->Enemies[i];
		if (enemy->headhunter) {
			huntedEnemiesLeft++;
		}
	}
	checkEnemyHealth(gameState);
	if (huntedEnemiesLeft == 0) {
		return true;
	}
	return false;
}

//Boss Remaining
bool Logic::bossRemaining(Gamestate *gameState) {
	int boss = 0;
	for (int i = 0; i < (int)gameState->Enemies.size(); i++) {
		EnemyUnit* enemy = &gameState->Enemies[i];
		if (enemy->boss) {
			boss++;
		}
	}
	checkEnemyHealth(gameState);
	if (boss == 0) {
		return true;
	}
	return false;
}

//Payload check
int Logic::payloadCollected(Gamestate *gameState) {
	int payload = 0;
	for (int i = 0; i < (int)gameState->PowerUps.size(); i++) {
		PowerUp* powerUp = &gameState->PowerUps[i];
		if (powerUp->type == 6) {
			if (powerUp->active) {
				payload++;
			}
		}
	}
	return payload;
}

//PowerUps
void Logic::spawnPowerUps(Gamestate *gameState) {

	for (int i = 0; i < gameState->wave*2 + 2; i++) {
		switch (i % 5) {
		case 0: //Spawn Point 1
			gameState->SpawnDynamicObject(1, 1.f - ((i + gameState->wave) * 10.f), 1.f, 1.f - ((i + gameState->wave) * 10.f), 0, 0, 0);
			break;
		case 1: //Spawn Point 2
			gameState->SpawnDynamicObject(rand() % 5 + 1, -100.f - ((i + gameState->wave) * 20.f), 5.25f, -100.f + ((i + gameState->wave) * 20.f), 0, 0, 0);
			break;
		case 2: //Spawn Point 3
			gameState->SpawnDynamicObject(rand() % 5 + 1, 100.f + ((i + gameState->wave) * 20.f), -5.25f, 100.f - ((i + gameState->wave) * 20.f), 0, 0, 0);
			break;
		case 3: //Spawn Point 4
			gameState->SpawnDynamicObject(rand() % 5 + 1, -100.f - ((i + gameState->wave) * 20.f), 1.f, 100.f - ((i + gameState->wave) * 20.f), 0, 0, 0);
			break;
		case 4: //Spawn Point 5
			gameState->SpawnDynamicObject(rand() % 5 + 1, 53.f + ((i + gameState->wave) * 20.f), 1.f, -35.f + ((i + gameState->wave) * 20.f), 0, 0, 0);
			break;
		}
	}
}

//Game Modes
void Logic::modeSelection(Gamestate *gameState) {
	switch (rand() % 5 + 1) {
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
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 1: //Spawn Point 2
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 145.f + (i * -5.f), 21.f, 155.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 2: //Spawn Point 3
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -90.f + (i * 5.f), 21.f, 155.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 3: //Spawn Point 4
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -137.f + (i * 5.f), 9.f, 10.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 4: //Spawn Point 5
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -137.f + (i * 5.f), 9.f, -30.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 5: //Spawn Point 6
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -100.f + (i * 5.f), -2.f, -132.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		}
	}
}

//Checkpoint
void Logic::checkpoint(Gamestate *gameState) {
	for (int i = 0; i < gameState->wave + 3; i++) {
		switch (i % 4) {
		case 0:
			gameState->SpawnDynamicObject(0, 45.f + (i * 5.f), 0.f, 45.f + (i * 10.f), 0, 0, 0);
			break;
		case 1:
			gameState->SpawnDynamicObject(0, -25.f - (i * 5.f), 0.f, 25.f + (i * 10.f), 0, 0, 0);
			break;
		case 2:
			gameState->SpawnDynamicObject(0, 45.f + (i * 15.f), 0.f, -45.f - (i * 15.f), 0, 0, 0);
			break;
		case 3:
			gameState->SpawnDynamicObject(0, -30.f - (i * 10.f), 0.f, -30.f - (i * 10.f), 0, 0, 0);
			break;
		}
	}

	//Spawn Enemies
	for (int i = 0; i < gameState->wave; i++) {
		switch (i % 6) {
		case 0: //Spawn Point 1
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 150.f + (i * -5.f), -2.f, -85.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 1: //Spawn Point 2
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 145.f + (i * -5.f), 21.f, 155.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 2: //Spawn Point 3
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -90.f + (i * 5.f), 21.f, 155.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 3: //Spawn Point 4
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -137.f + (i * 5.f), 9.f, 10.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 4: //Spawn Point 5
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -137.f + (i * 5.f), 9.f, -30.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 5: //Spawn Point 6
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -100.f + (i * 5.f), -2.f, -132.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		}
	}
}

//Payload
void Logic::payload(Gamestate *gameState) {
	payloadcollected = false;
	gameState->SpawnDynamicObject(6, 10.f, 0.f, 10.f, 0, 0, 0);
}

//Head hunter
void Logic::headHunter(Gamestate *gameState) {
	for (int i = 0; i < gameState->wave + 1; i++) {
		switch (i % 6) {
		case 0: //Spawn Point 1
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 150.f + (i * -5.f), -2.f, -85.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 1: //Spawn Point 2
			gameState->SpawnEnemy(rand() % 4 + 1, 1, 145.f + (i * -5.f), 21.f, 155.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			gameState->Enemies.back().headhunter = true;
			break;
		case 2: //Spawn Point 3
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -90.f + (i * 5.f), 21.f, 155.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 3: //Spawn Point 4
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -137.f + (i * 5.f), 9.f, 10.f + (i * -5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 4: //Spawn Point 5
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -137.f + (i * 5.f), 9.f, -30.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 5: //Spawn Point 6
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -100.f + (i * 5.f), -2.f, -132.f + (i * 5.f), 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		}
	}
}

//Boss battle
void Logic::bossBattle(Gamestate *gameState) {
		switch (gameState->wave) {
		case 1:
			gameState->SpawnEnemy(5, 0, -137.f, 9.f, 10.f, 0, 0, 0);
			gameState->Enemies.back().health = 50.f;
			gameState->Enemies.back().maxhealth = 50.f;
			gameState->Enemies.back().boss = true;
			break;
		case 2:
			gameState->SpawnEnemy(5, 0, -137.f, 9.f, 10.f, 0, 0, 0);
			gameState->Enemies.back().health = 75.f;
			gameState->Enemies.back().maxhealth = 75.f;
			gameState->Enemies.back().boss = true;
			break;
		case 3:
			gameState->SpawnEnemy(5, 0, -137.f, 9.f, 10.f, 0, 0, 0);
			gameState->Enemies.back().health = 100.f;
			gameState->Enemies.back().maxhealth = 100.f;
			gameState->Enemies.back().boss = true;
			break;
		case 4:
			gameState->SpawnEnemy(5, 0, -137.f, 9.f, 10.f, 0, 0, 0);
			gameState->Enemies.back().health = 150.f;
			gameState->Enemies.back().maxhealth = 150.f;
			gameState->Enemies.back().boss = true;
			break;
		case 5:
			gameState->SpawnEnemy(5, 0, -137.f, 9.f, 10.f, 0, 0, 0);
			gameState->Enemies.back().health = 200.f;
			gameState->Enemies.back().maxhealth = 200.f;
			gameState->Enemies.back().boss = true;
			break;
		default:
			gameState->SpawnEnemy(5, 0, -137.f, 9.f, 10.f, 0, 0, 0);
			gameState->Enemies.back().health = 100.f;
			gameState->Enemies.back().maxhealth = 100.f;
			gameState->Enemies.back().boss = true;
			break;
		}
}
