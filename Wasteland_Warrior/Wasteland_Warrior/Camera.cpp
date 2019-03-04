#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Gamestate.h"
#include <math.h>
#include <iostream>


using namespace std;
using namespace glm;

const float MAX_VERT = 0.99f;

Camera::Camera(Gamestate* newGamestate) {
	dir = glm::vec3(0, 0, -1);
	right = glm::vec3(1, 0, 0);
	up = glm::vec3(0, 1, 0);

	pos = glm::vec3(0,0,0);
	radius = 1.f;

	gameState = newGamestate;
}

glm::mat4 Camera::viewMatrix() const {

	glm::mat4 viewMatrix;

	glm::vec3 car = gameState->playerVehicle.position;
	glm::vec3 cam = gameState->playerVehicle.position; 
	
	//rotation angle based on input
	float angle;
	if (gameState->rightStickX == 0) {
		angle = gameState->cameraAngle;
	}
	else {
		angle = gameState->rightStickX;
	}

	glm::vec2 direction = gameState->playerVehicle.direction;
	glm::vec2 newDirection;

	newDirection.x = (direction.x * cos(angle)) - (direction.y * sin(angle));
	newDirection.y = (direction.y * cos(angle)) + (direction.x * sin(angle));

	float lagSensitivity = 0.0;

	float xVal = newDirection.x / newDirection.length();
	float yVal = newDirection.y / newDirection.length();

	cam.x = -15 * xVal + car.x;
	cam.z = -15 * yVal + car.z - (gameState->playerVehicle.acceleration * lagSensitivity);
	cam.y = cam.y + 2;

	viewMatrix = glm::lookAt(cam, car, up);

	return viewMatrix;
}


