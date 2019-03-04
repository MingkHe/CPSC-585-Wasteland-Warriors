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

	float lagSensitivity = 1.0;
	float distanceBehindCar = 12 + (gameState->playerVehicle.speed * lagSensitivity); //change to acceleration once its being captured acuratly
	float distanceAboveCar = 3;

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

	float xVal = newDirection.x / newDirection.length();
	float yVal = newDirection.y / newDirection.length();

	cam.x = -distanceBehindCar *xVal + car.x;
	cam.z = -distanceBehindCar *yVal + car.z;
	cam.y = cam.y + distanceAboveCar;

	viewMatrix = glm::lookAt(cam, car, up);

	return viewMatrix;
}


