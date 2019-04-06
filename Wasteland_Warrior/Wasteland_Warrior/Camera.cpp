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
		+ (gameState->playerVehicle.heading.z - gameState->playerVehicle.direction.z)*(gameState->playerVehicle.heading.z - gameState->playerVehicle.direction.z);
	if (gameState->button == "REVERSE") {
		forward = -1;
	}

	//Third Person
	float distanceBehindCar = forward * (7 + (gameState->playerVehicle.speed * 0.25f));
	float distanceAboveCar = 0.75;

	//Hood Cam
	if (gameState->view == 1) {
		distanceBehindCar = -0.5f;
		distanceAboveCar = 0.01f;
	}
	//Cockpit
	else if(gameState->view == 2) {
		distanceBehindCar = forward * (20 + (gameState->playerVehicle.speed * 1.00f));
		distanceAboveCar = 10.0f;
	}

	//Rotation
	float angle;
	if (gameState->controller == false) {
		angle = pow(gameState->cameraAngle, 5) * 0.20f;
		if (gameState->view > 0) {
			angle = -angle;
		}
	}
	else {
		angle = pow(gameState->rightStickX, 5) * 1.5f;
	}

	//Direction
	glm::vec3 direction = gameState->playerVehicle.direction;
	glm::vec2 newDirection;

	newDirection.x = (direction.x * cos(angle)) - (direction.z * sin(angle));
	newDirection.y = (direction.z * cos(angle)) + (direction.x * sin(angle));

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
		//car.y = car.y + distanceAboveCar;
	}

	return glm::lookAt(cam, car, up);
}

glm::mat4 Camera::backviewMatrix() const {

	glm::vec3 car = gameState->playerVehicle.position;
	glm::vec3 cam = gameState->playerVehicle.position;

	float dir_head_discr = (gameState->playerVehicle.heading.x - gameState->playerVehicle.direction.x)*(gameState->playerVehicle.heading.x - gameState->playerVehicle.direction.x)
		+ (gameState->playerVehicle.heading.z - gameState->playerVehicle.direction.z)*(gameState->playerVehicle.heading.z - gameState->playerVehicle.direction.z);


	float distanceBehindCar = 4.f;
	float distanceAboveCar = 0.f;

	//Rotation
	float angle;
	if (gameState->controller == false) {
		angle = pow(gameState->cameraAngle, 5) * 0.20f;
		if (gameState->view > 0) {
			angle = -angle;
		}
	}
	else {
		angle = pow(gameState->rightStickX, 5) * 1.5f;
	}

	//Direction
	glm::vec3 direction = gameState->playerVehicle.direction;
	glm::vec2 newDirection;

	newDirection.x = (direction.x * cos(angle)) - (direction.z * sin(angle));
	newDirection.y = (direction.z * cos(angle)) + (direction.x * sin(angle));

	float xVal = newDirection.x / newDirection.length();
	float yVal = newDirection.y / newDirection.length();

	//Third Person
	cam.x = -distanceBehindCar * xVal + car.x;
	cam.z = -distanceBehindCar * yVal + car.z;
	cam.y = cam.y + distanceAboveCar;

	car.x = -distanceBehindCar * xVal + cam.x;
	car.z = -distanceBehindCar * yVal + cam.z;
	car.y = car.y + distanceAboveCar - 0.5f;

	return glm::lookAt(cam, car, up);
}
