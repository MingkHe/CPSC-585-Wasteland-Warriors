#include "EnemyUnit.h"

EnemyUnit::EnemyUnit()
{
}

EnemyUnit::EnemyUnit(int newPhysicsIndex, int newSceneObjectIndex) {
	this->health = 100;
	physicsIndex = newPhysicsIndex;
	sceneObjectIndex = newSceneObjectIndex;

	lastMotionTime = std::chrono::system_clock::now();
}

EnemyUnit::~EnemyUnit()
{
}

bool EnemyUnit::CheckForStuck(){
	if (recoveryMode = false) {			//Car is not currently marked as stuck
		if (speed > 0.1) {				//Check if car is moving
			lastMotionTime = std::chrono::system_clock::now();
			return false;
		}

		auto currentTime = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = currentTime - lastMotionTime;

		if (elapsed_seconds.count() > 2) {	//Car is moving so check if stuck for more than 2 seconds
			recoveryMode = true;
			return true;
		}

		return false;
	}

	else {
		auto currentTime = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = currentTime - lastMotionTime;

		if (elapsed_seconds.count() > 4) {	//For 2 more seconds remain in recovery mode
			recoveryMode = false;
			lastMotionTime = currentTime;
			return false;
		}

		return true;
	}
}