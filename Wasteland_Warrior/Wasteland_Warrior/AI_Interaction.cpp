#include "AI_Interaction.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>


AI_Interaction::AI_Interaction() {}

AI_Interaction::AI_Interaction(Gamestate* newGameState)
{
	gameState = newGameState;
}


AI_Interaction::~AI_Interaction()
{
}

int AI_Interaction::Update()
{
	glm::vec2 targetPosition = { 0.0f, 0.0f };
	glm::vec3 PlayerPosition = gameState->playerVehicle.position;

	for (int i = 0; i < (int)(gameState->Enemies.size()); i++) {
		//Enemy information
		EnemyUnit* enemy = &gameState->Enemies[i];
		glm::vec3 enemyHeading = glm::normalize(enemy->direction);
		glm::vec2 enemyPosition = {enemy->position.x, enemy->position.z};

		//std::cout << "Enemy type: " << enemy.type << std::endl;

		float distFromDest = glm::length(enemy->destination - enemyPosition);
		//std::cout << "Destination: (" << enemy->destination.x << "," << enemy->destination.y << ")" << std::endl;
		//std::cout << "Location: (" << enemyPosition.x << "," << enemyPosition.y << ")" << std::endl;
		//std::cout << "distFromDest = " << distFromDest << std::endl;

		float triggerRadious = 10.0f;

		//Destination information
		if (enemy->AIType == 0)
			enemy->destination = glm::vec2(PlayerPosition.x, PlayerPosition.z);

		else if (enemy->AIType == 1) {
			if ((enemy->destination.x == 0 && enemy->destination.y == 0) ||
				distFromDest < triggerRadious)
			{
				int levelLength = 300;
				int levelWidth = 300;
				//Enemy has arived at destination, new destination created
				enemy->destination = glm::vec2((rand()% levelLength)-(levelLength/2), (rand()% levelWidth)-(levelWidth/2));
				//std::cout << "Destination reached, new destination: (" << enemy->destination.x << "," << enemy->destination.y << ")" << std::endl;
			}
		}

		else if (enemy->AIType == 2) {
			if (distFromDest < triggerRadious)
			{
				enemy->AIType = enemy->AITypeRevert;
				enemy->recoveryMode = false;
			}
		}

		//Set target
		targetPosition = enemy->destination;

		//Determin turn direction
		float enemyRotation = glm::atan(enemyHeading.z / enemyHeading.x);
		if (enemyHeading.x < 0 && enemyRotation > 0)
			enemyRotation = enemyRotation -(float)M_PI;

		else if (enemyHeading.x < 0 && enemyRotation < 0)
			enemyRotation = enemyRotation +(float)M_PI;



		glm::vec2 targetVector = normalize(targetPosition - enemyPosition);
		float targetRotation = glm::atan(targetVector.y / targetVector.x);


		if (targetVector.x < 0 && targetRotation > 0) {
			targetRotation = targetRotation-(float)M_PI;
		}

		else if (targetVector.x < 0 && targetRotation < 0)
			targetRotation = targetRotation+(float)M_PI;


		float relativeRotation;
		glm::vec2 controllInput;		//{(Speed -1 -> 1), (Turning -1 (left) -> 1 (right))} 
		if (enemyRotation == targetRotation) {
			controllInput = {1.0f, 0.0f};			//Dont turn
		}
		else {  //target - enemy   or 
			relativeRotation = targetRotation - enemyRotation;

			if (relativeRotation > (M_PI))
				relativeRotation -= (2.f * (float)M_PI);

			if (relativeRotation <= (-M_PI))
				relativeRotation += (2.f * (float)M_PI);


			if (relativeRotation > 0.0f) {
				controllInput = { 0.8f, -1.0f };	//Turn left
			}
			else {
				controllInput = { 0.8f, 1.0f };		//Turn right
			}
		}

		gameState->pathfindingInputs[i] = controllInput;

	} 
	return 0;
}
