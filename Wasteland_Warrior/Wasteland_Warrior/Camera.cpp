#include "Camera.h"
#include "Gamestate.h"

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

	glm::vec3 car = gameState->playerVehicle.position;
	glm::vec3 cam = gameState->playerVehicle.position; 

	float distanceBehindCar = 10 + (gameState->playerVehicle.speed * 0.25);
	float distanceAboveCar = 2.75;

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

	return glm::lookAt(cam, car, up);
}


