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
	int forward = 1;

	float dir_head_discr = (gameState->playerVehicle.heading.x - gameState->playerVehicle.direction.x)*(gameState->playerVehicle.heading.x - gameState->playerVehicle.direction.x)
		+ (gameState->playerVehicle.heading.y - gameState->playerVehicle.direction.y)*(gameState->playerVehicle.heading.y - gameState->playerVehicle.direction.y);
	if (dir_head_discr > .5 && gameState->playerVehicle.speed > 5) {//change the 5 to a 4 maybe?
		forward = -1;
	}

	//Third Person
	float distanceBehindCar = forward * (10 + (gameState->playerVehicle.speed * 0.25));
	float distanceAboveCar = 2.75;

	//Hood Cam
	if (gameState->view == 1) {
		distanceBehindCar = -0.5;
		distanceAboveCar = 0.75;
	}
	//Cockpit
	else if(gameState->view == 2) {
		distanceBehindCar = 0.5;
		distanceAboveCar = 0.4;
	}

	//Rotation
	float angle;
	if (gameState->controller == false) {
		angle = pow(gameState->cameraAngle, 5) * 0.20;
		if (gameState->view > 0) {
			angle = -angle;
		}
	}
	else {
		angle = pow(gameState->rightStickX, 5) * 1.5;
	}

	//Direction
	glm::vec2 direction = gameState->playerVehicle.direction;
	glm::vec2 newDirection;

	newDirection.x = (direction.x * cos(angle)) - (direction.y * sin(angle));
	newDirection.y = (direction.y * cos(angle)) + (direction.x * sin(angle));

	float xVal = newDirection.x / newDirection.length();
	float yVal = newDirection.y / newDirection.length();

	//Third Person
	cam.x = -distanceBehindCar *xVal + car.x;
	cam.z = -distanceBehindCar *yVal + car.z;
	cam.y = cam.y + distanceAboveCar;

	//Hood Cam
	if (gameState->view == 1) {
		car.x = -distanceBehindCar * xVal + cam.x;
		car.z = -distanceBehindCar * yVal + cam.z;
		car.y = car.y + distanceAboveCar;
	}
	//Cockpit
	else if (gameState->view == 2) {
		car.y = car.y + distanceAboveCar;
	}

	return glm::lookAt(cam, car, up);
}


