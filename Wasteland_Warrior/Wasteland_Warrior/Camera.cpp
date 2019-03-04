#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
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

	glm::vec3 car = gameState->playerVehicle.position;
	glm::vec3 cam = gameState->playerVehicle.position;
	float distanceBehindCar = 12;
	float distanceAboveCar = 3;
	//Position behind car

	float lagSensitivity = 0.0;

	//Rotate camera based on direction
	float xVal = gameState->playerVehicle.direction.x / gameState->playerVehicle.direction.length();
	float yVal = gameState->playerVehicle.direction.y / gameState->playerVehicle.direction.length();

	cam.x = (-distanceBehindCar + (gameState->playerVehicle.acceleration * lagSensitivity)) *xVal+car.x;
	cam.z = (-distanceBehindCar + (gameState->playerVehicle.acceleration * lagSensitivity)) *yVal+car.z ;
	cam.y = cam.y + distanceAboveCar;

	glm::mat4 viewMatrix = glm::lookAt(cam, car, up);
	return viewMatrix;
}


