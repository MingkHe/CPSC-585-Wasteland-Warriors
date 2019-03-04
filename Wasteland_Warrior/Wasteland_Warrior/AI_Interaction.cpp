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
	//This will later be replaced by an arbitrary point passed in
	glm::vec3 EntityPosition = gameState->playerVehicle.position;
	glm::vec2 targetPosition = { EntityPosition.x, EntityPosition.z};


	for (int i = 0; i < gameState->Enemies.size(); i++) {

		EnemyUnit enemy = gameState->Enemies[i];
		glm::vec2 enemyHeading = glm::normalize(enemy.direction);
		glm::vec2 enemyPosition = {enemy.position.x, enemy.position.z};

		std::cout << "I am at: [" << enemyPosition.x << "," << enemyPosition.y << "] and I would like to get to: [" << targetPosition.x << "," << targetPosition.y << "]" << std::endl;

		float enemyRotation = glm::atan(enemyHeading.y / enemyHeading.x);
		if (enemyHeading.x < 0 && enemyRotation > 0)
			enemyRotation -= M_PI;

		if (enemyHeading.x < 0 && enemyRotation < 0)
			enemyRotation += M_PI;



		std::cout << "enemyHeading: [" << enemyHeading.x << "," << enemyHeading.y << "]" << std::endl;
		std::cout << "enemyRotation in rad: " << enemyRotation << std::endl;

		glm::vec2 targetVector = normalize(targetPosition - enemyPosition);
		float targetRotation = glm::atan(targetVector.y / targetVector.x);
		if (targetVector.x < 0 && targetRotation > 0)
			targetRotation -= M_PI;

		if (targetVector.x < 0 && targetRotation < 0)
			targetRotation += M_PI;



		std::cout << "targetVector: [" << targetVector.x << "," << targetVector.y << "]" << std::endl;
		std::cout << "targetRotation in rad: " << targetRotation << std::endl;

		float relativeRotation;
		glm::vec2 controllInput;		//{(Speed -1 -> 1), (Turning -1 (left) -> 1 (right))} 
		if (enemyRotation == targetRotation) {
			controllInput = {1.0f, 0.0f};			//Dont turn
		}
		else {  //target - enemy   or 
			relativeRotation = targetRotation - enemyRotation;

			std::cout << "relativeRotation is: " << relativeRotation << std::endl;

			if (relativeRotation > (M_PI))
				relativeRotation -= (2 * M_PI);

			if (relativeRotation <= (-M_PI))
				relativeRotation += (2 * M_PI);


			if (relativeRotation > 0.0f) {
				controllInput = { 1.0f, -1.0f };	//Turn left
			}
			else {
				controllInput = { 1.0f, 1.0f };		//Turn right
			}
		}

		gameState->pathfindingInputs[i] = controllInput;

	} 
	return 0;
}
