#include "AI_Interaction.h"
#define _USE_MATH_DEFINES
#include <math.h>


AI_Interaction::AI_Interaction()
{
}


AI_Interaction::~AI_Interaction()
{
}

int AI_Interaction::Update(Gamestate* GameState)
{
	//This will later be replaced by an arbitrary point passed in
	glm::vec3 playerPosition = GameState->playerVehicle.position;
	glm::vec2 targetPosition = { playerPosition.x, playerPosition.z};

	for (int i = 0; i < GameState->Enemies.size(); i++) {
		EnemyUnit enemy = GameState->Enemies[i];
		glm::vec2 enemyHeading = enemy.direction;
		glm::vec2 enemyPosition = {enemy.position.x, enemy.position.z};

		float enemyRotation = glm::atan(enemyHeading.y / enemyHeading.x);
		if (enemyHeading.y < 0)
			enemyRotation += M_PI;

		glm::vec2 targetVector = targetPosition - enemyPosition;
		float targetRotation = glm::atan(targetVector.y / targetVector.x);
		if (targetVector.y < 0)
			targetRotation += M_PI;



		float relativeRotation;
		glm::vec2 controllInput;		//{(Speed -1 -> 1), (Turning -1 (left) -> 1 (right))} 
		if (enemyRotation == targetRotation) {
			controllInput = {1.0f, 0.0f};			//Dont turn
		}
		else {
			relativeRotation = targetRotation + ((2 * M_PI)-enemyRotation);
			if (relativeRotation >= (2 * M_PI))
				relativeRotation - (2 * M_PI);

			if (relativeRotation < M_PI) {
				controllInput = { 1.0f, -1.0f };	//Turn left
			}
			else {
				controllInput = { 1.0f, 1.0f };		//Turn right
			}
		}

		//Pass controllInput to something that controlls the AI vehicles

	} 
	return 0;
}
