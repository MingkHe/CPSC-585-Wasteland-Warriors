#include "EnemyUnit.h"
#include <iostream>
#include <glm/glm.hpp>


EnemyUnit::EnemyUnit()
{
}

EnemyUnit::EnemyUnit(int newPhysicsIndex, int newSceneObjectIndex) {
	this->health = 100;
	physicsIndex = newPhysicsIndex;
	sceneObjectIndex = newSceneObjectIndex;

	struct timeb currentTime;
	ftime(&currentTime);
	lastMotionTime = (int)(currentTime.time);
}

EnemyUnit::~EnemyUnit()
{
}

bool EnemyUnit::CheckForStuck(){
	struct timeb currentTime;
	ftime(&currentTime);
	double elapsed_seconds = (double)(currentTime.time) - (double)(lastMotionTime);

	//std::cout << "Current time: " << currentTime.time << "     and last movement was at: " << lastMotionTime << std::endl;
	//std::cout << "Elapsed stuck seconds = " << elapsed_seconds << std::endl;

	if (recoveryMode == false) {			//Car is not currently marked as stuck
		//std::cout << "Recovery mode OFF" << std::endl;
		if (speed > 1) {				//Check if car is moving
			lastMotionTime = (int)(currentTime.time);
			//std::cout << "lastMotionTime updated to: " << lastMotionTime << std::endl;
			return false;
		}

		if (elapsed_seconds >= 2) {	//Car is moving so check if stuck for more than 2 seconds
			recoveryMode = true;
			//std::cout << "Recovery mode turned ON" << std::endl;
			return true;
		}

		return false;
	}

	else {
		//std::cout << "Recovery mode ON" << std::endl;
		if (elapsed_seconds >= 4) {	//For 2 more seconds remain in recovery mode

			if (speed < 1)			//If still stuck
				forceRelocate = true;

			recoveryMode = false;

			destination = glm::vec2(position.x + (rand() % 60) - 30, position.z + (rand() % 60) - 30);		//Move to a new location before chasing player
			
			while(glm::dot(normalize(destination), normalize(glm::vec2(direction.x, direction.z))) >= 0.8){
				destination = glm::vec2(position.x + (rand() % 60) - 30, position.z + (rand() % 60) - 30);		//Select another location
			}

			if(AITypeRevert == -1)
				AITypeRevert = AIType;

 			AIType = 2;
			//std::cout << "Recovery mode turned OFF" << std::endl;
			lastMotionTime = (int)(currentTime.time);
			//std::cout << "lastMotionTime updated to: " << lastMotionTime << std::endl;
			return false;
		}

		return true;
	}

	return false;
}
