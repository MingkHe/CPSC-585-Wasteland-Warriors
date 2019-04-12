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
	this->collected = 0;
}

Logic::~Logic()
{
}

void Logic::Update(Gamestate *gameState)
{

	std::cout << "x " << gameState->playerVehicle.position.x << "y " << gameState->playerVehicle.position.y << "z " << gameState->playerVehicle.position.z << std::endl;

		//Randomization
		srand((unsigned int)time(NULL));

		//Score
		score++;

		//Restart

		if (gameState->restart) {
			if (gameState->startup) {
				if (gameState->resetCar) {
					//Reset Car
					gameState->physics_Controller->setPosition(gameState->playerVehicle.physicsIndex, glm::vec3{ 100, 2, -170 });
					gameState->playerVehicle.health = 100;
					gameState->playerVehicle.maxhealth = 100;
					gameState->playerVehicle.damageMultiplier = 1;
					gameState->playerVehicle.armor = 0.0;
					gameState->view = 0;
					gameState->resetCar = false;

					//Reset Enemies
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
					powerUps(gameState);
					gameState->wave = -1;
					waveBreak = -1;
					gameState->gameMode = "";
					gameState->weaponState = false;

					//Truck
					for (int i = 0; i < (int)gameState->StaticObjects.size(); i++) {
						Object* truck = &gameState->StaticObjects[i];
						if (truck->type == 7) {
							int offset = truck->physicsIndex;
							glm::mat4 transformMatrix = glm::mat4(
								2.f, 0.f, 0.f, 0.f,
								0.f, 2.f, 0.f, 0.f,
								0.f, 0.f, 2.f, 0.f,
								0.f, 0.f, 173.f, 1.f
							);
							for (int s = 0; s < gameState->scene->allWorldCompObjects[truck->sceneObjectIndex].subObjectsCount; s++) {
								gameState->scene->allWorldCompObjects[truck->sceneObjectIndex].subObjects[s].transform = transformMatrix;
							}
						}
					}

					//Boulder
					for (int i = 0; i < (int)gameState->StaticObjects.size(); i++) {
						Object* boulder = &gameState->StaticObjects[i];
						if (boulder->type == 14) {
							int offset = boulder->physicsIndex;
							glm::mat4 transformMatrix = glm::mat4(
								2.f, 0.f, 0.f, 0.f,
								0.f, 2.f, 0.f, 0.f,
								0.f, 0.f, 2.f, 0.f,
								0.f, -500.f, 0.f, 1.f
							);
							for (int s = 0; s < gameState->scene->allWorldCompObjects[boulder->sceneObjectIndex].subObjectsCount; s++) {
								gameState->scene->allWorldCompObjects[boulder->sceneObjectIndex].subObjects[s].transform = transformMatrix;
							}
						}
					}
				}

				if ((gameState->playerVehicle.position.z > -165 && gameState->playerVehicle.position.z < -164 && gameState->playerVehicle.position.x > 90 && gameState->playerVehicle.position.x < 110)) {
					gameState->startup = false;

					//Boulder
					for (int i = 0; i < (int)gameState->StaticObjects.size(); i++) {
						Object* boulder = &gameState->StaticObjects[i];
						if (boulder->type == 14) {
							int offset = boulder->physicsIndex;
							glm::mat4 transformMatrix = glm::mat4(
								2.f, 0.f, 0.f, 0.f,
								0.f, 2.f, 0.f, 0.f,
								0.f, 0.f, 2.f, 0.f,
								100.f, 2.f, -170.f, 1.f
							);
							for (int s = 0; s < gameState->scene->allWorldCompObjects[boulder->sceneObjectIndex].subObjectsCount; s++) {
								gameState->scene->allWorldCompObjects[boulder->sceneObjectIndex].subObjects[s].transform = transformMatrix;
							}
						}
					}
				}
			}
			else {

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
				gameState->weaponState = false;

			}
		}


		if (gameState->startup == false) {

		//Player has lost all health
		if ((gameState->playerVehicle.health <= 0)) {
			gameState->UIMode = "Lose";
			gameState->ui_gameplay = false;
			gameState->restart = true;
			gameState->score = ((score / 25) * 10) + gameState->enemyscore;
			gameState->scoreTime = score / 60;
			gameState->startup = true;
			gameState->resetCar = true;
		}
		else {

			//Player has beaten all 5 waves
			if (gameState->wave == 7 || waveBreak == 7) {
				gameState->UIMode = "Win";
				gameState->ui_gameplay = false;
				gameState->restart = true;
				gameState->score = (int)(2160010000 / (score + 1)) + gameState->enemyscore;
				gameState->scoreTime = (int)(score / 60);
				gameState->startup = true;
				gameState->resetCar = true;
			}
			else {
					//Wave
					if (gameState->wave == waveBreak) {
						if (waveFinished(gameState)) {
							//Reset Enemies
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
							powerUps(gameState);
							gameState->wave++;
						}
					}

					//Break
					else {
						gameState->breakSeconds = breakTime / 60;
						if (breakTime <= 0) {
							waveBreak++;
							breakTime = 600;
							if (gameState->wave == 5) {
								boss(gameState);
								gameState->gameMode = "Boss Battle";
							}
							else if (gameState->wave == 6) {
								payload(gameState);
								gameState->gameMode = "End Game";
							}
							else {
								modeSelection(gameState);
							}
							gameState->modeText = true;
							gameState->textTime = 10 * 60;
						}
						breakTime--;
					}
				}
			}
		}

		//Stabilize powerups and checkpoints
		for (int i = 0; i < (int)gameState->PowerUps.size(); i++) {
			PowerUp* powerUp = &gameState->PowerUps[i];
			if (powerUp->active) {
				gameState->resetOrientation(powerUp->physicsIndex);
			}
		}
}

//Wave End
bool Logic::waveFinished(Gamestate *gameState) {

	bool objective = false;

	checkEnemyHealth(gameState);

	if (gameState->gameMode == "Checkpoint") {
		objective = checkpointsCollected(gameState);
	}
	else if (gameState->gameMode == "Head Hunter") {
		objective = huntedEnemiesRemaining(gameState);
	}
	else if (gameState->gameMode == "Boss Battle") {
		objective = bossRemaining(gameState);
	}
	else if (gameState->gameMode == "Payload") {
		objective = payloadCollected(gameState);
	}
	else if (gameState->gameMode == "Survival") {
		objective = true;
	}
	else if (gameState->gameMode == "End Game") {
		objective = endgame(gameState);
	}

	if (checkEnemyHealth(gameState) == 0 && objective) {
		gameState->weaponState = false;
		return true;
	}
	else if (checkEnemyHealth(gameState) > 0 && objective && gameState->gameMode != "Survival") {
		gameState->weaponState = true;
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
bool Logic::checkpointsCollected(Gamestate *gameState) {
	int checkpoints = 0;
	bool checkpointCollected = false;

	for (int i = 0; i < (int)gameState->PowerUps.size(); i++) {
		PowerUp* powerUp = &gameState->PowerUps[i];
		if (powerUp->type == 0) {
			if (powerUp->active) {
				checkpoints++;
			} 
		}
	}

	if (checkpoints <= 0) {
		collected++;
		if (gameState->checkpointsLeft > 0) {
			gameState->checkpointsLeft--;
		}
		checkpointCollected = true;
	}
	
	if (checkpointCollected && collected < gameState->wave && gameState->checkpointsLeft > 0) {
		checkpoint(gameState);
	}
	else if (gameState->checkpointsLeft <= 0) {
		collected = 0;
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
	if (boss == 0) {
		return true;
	}
	return false;
}

//Payload check
bool Logic::payloadCollected(Gamestate *gameState) {
		int checkpoints = 0;
		int payloads = 0;

		if (gameState->checkpointCollected) {
			collected++;
		}

		if (gameState->payloadCollected) {
			collected++;
		}

		if (gameState->checkpointCollected && collected < (gameState->wave * 2)) {
			if (gameState->checkpointsLeft > 0) {
				gameState->checkpointsLeft--;
			}
			gameState->checkpointCollected = false;
			payload(gameState);
		}

		if (gameState->payloadCollected && collected < (gameState->wave * 2)) {
			gameState->payloadCollected = false;
			checkpoint(gameState);
		}

		if (collected > (gameState->wave * 2)) {
			gameState->checkpointCollected = false;
			gameState->payloadCollected = false;
			return true;
		}

		return false;
}

//End Game
bool Logic::endgame(Gamestate *gameState) {

	if (gameState->payloadCollected) {
		if ((gameState->playerVehicle.position.z > 155 && gameState->playerVehicle.position.z < 156 && gameState->playerVehicle.position.x > -20 && gameState->playerVehicle.position.x < 20)) {
			gameState->explosions.push_back(Explosion(glm::vec3(0,0,173)));
			for (int i = 0; i < (int)gameState->StaticObjects.size(); i++) {
				Object* truck = &gameState->StaticObjects[i];
				if (truck->type == 7) {
					int offset = truck->physicsIndex;
					glm::mat4 transformMatrix = glm::mat4(
						2.f, 0.f, 0.f, 0.f,
						0.f, 2.f, 0.f, 0.f,
						0.f, 0.f, 2.f, 0.f,
						(-100 * offset), -500.f, -500.f, 1.f
					);
					for (int s = 0; s < gameState->scene->allWorldCompObjects[truck->sceneObjectIndex].subObjectsCount; s++) {
						gameState->scene->allWorldCompObjects[truck->sceneObjectIndex].subObjects[s].transform = transformMatrix;
					}
				}
			}
			if ((gameState->playerVehicle.position.z > 165 && gameState->playerVehicle.position.z < 166 && gameState->playerVehicle.position.x > -20 && gameState->playerVehicle.position.x < 20)) {
				return true;
			}
		}
	}

	return false;
}

//PowerUps
void Logic::powerUps(Gamestate *gameState) {
	gameState->SpawnDynamicObject(1, 1.f, 2.f, 1.f, 0, 0, 0);
	gameState->SpawnDynamicObject(rand() % 5 + 1, 24.f, 2.f, -155.f, 0, 0, 0);
	gameState->SpawnDynamicObject(rand() % 5 + 1, 16.f, 2.f, -55.f, 0, 0, 0);
	gameState->SpawnDynamicObject(rand() % 5 + 1, 115.f, 2.f, 24.f, 0, 0, 0);
	gameState->SpawnDynamicObject(rand() % 5 + 1, -100.f, 2.f, 77.f, 0, 0, 0);
}

//Game Modes
void Logic::modeSelection(Gamestate *gameState) {
	switch (rand() % 4 + 1) {
	case 1:
		enemies(gameState);
		gameState->gameMode = "Survival";
		break;
	case 2:
		headHunter(gameState);
		enemies(gameState);
		gameState->gameMode = "Head Hunter";
		break;
	case 3:
		collected = 0;
		payload(gameState);
		enemies(gameState);
		gameState->gameMode = "Payload";
		gameState->checkpointsLeft = gameState->wave;
		break;
	case 4:
		collected = 0;
		checkpoint(gameState);
		enemies(gameState);
		gameState->gameMode = "Checkpoint";
		gameState->checkpointsLeft = gameState->wave;
		break;
	}
}

//Enemies
void Logic::enemies(Gamestate *gameState) {
	for (int i = 0; i < gameState->wave+1; i++) {
		switch (i % 4) {
		case 0:
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 171.f, 2.f, -172.f, 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 1:
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 158.f, 2.f, 74.f, 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 2:
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -173.f, 20.f, 173.f, 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 3:
			gameState->SpawnEnemy(rand() % 4 + 1, 0, -178.f, 8.f, -171.f, 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		case 4:
			gameState->SpawnEnemy(rand() % 4 + 1, 0, 50.f, 2.f, -180.f, 0, 0, 0);
			gameState->Enemies.back().health = 25.f;
			gameState->Enemies.back().maxhealth = 25.f;
			break;
		}
	}
}

//Checkpoint
void Logic::checkpoint(Gamestate *gameState) {
		switch (rand() % 6) {
		case 0:
			gameState->SpawnDynamicObject(0, 45.f, 0.5f, 45.f, 0, 0, 0);
			break;
		case 1:
			gameState->SpawnDynamicObject(0, 152.f, -5.f, -94.f, 0, 0, 0);
			break;
		case 2:
			gameState->SpawnDynamicObject(0, 133.f, 20.f, 176.f, 0, 0, 0);
			break;
		case 3:
			gameState->SpawnDynamicObject(0, -90.f, 20.f, 165.f, 0, 0, 0);
			break;
		case 4:
			gameState->SpawnDynamicObject(0, -151.f, 8.f, -2.f, 0, 0, 0);
			break;
		case 5:
			gameState->SpawnDynamicObject(0, -110.f, -5.f, -120.f, 0, 0, 0);
			break;
	}
}

//Payload
void Logic::payload(Gamestate *gameState) {
	gameState->SpawnDynamicObject(6, 10.f, 1.f, 10.f, 0, 0, 0);
}

//Head hunter
void Logic::headHunter(Gamestate *gameState) {
	gameState->SpawnEnemy(rand() % 4 + 1, 1, 163.f, 20.f, 167.f, 0, 0, 0);
	gameState->Enemies.back().health = 25.f;
	gameState->Enemies.back().maxhealth = 25.f;
	gameState->Enemies.back().headhunter = true;
}

//Boss
void Logic::boss(Gamestate *gameState) {
	gameState->SpawnEnemy(5, 0, -165.f, 2.f, 74.f, 0, 0, 0);
	gameState->Enemies.back().health = 200.f;
	gameState->Enemies.back().maxhealth = 200.f;
	gameState->Enemies.back().boss = true;

}
