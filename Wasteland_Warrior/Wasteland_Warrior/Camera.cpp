#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Gamestate.h"
#include <math.h>

using namespace std;
using namespace glm;

const float MAX_VERT = 0.99f;

Camera::Camera(Gamestate* newGamestate) {
	dir = glm::vec3(0, 0, -1);
	right = glm::vec3(1, 0, 0);
	up = glm::vec3(0, 1, 0);

	pos = glm::vec3(0);
	radius = 1.f;

	gameState = newGamestate;
}

glm::mat4 Camera::viewMatrix() const {

	glm::vec3 car = gameState->playerVehicle.position;
	glm::vec3 cam = gameState->playerVehicle.position;

	//Position behind car
	cam.z = cam.z - 25;
	cam.y = cam.y + 10;

	//Rotate camera based on direction
	float angle = atan(gameState->playerVehicle.direction.y / gameState->playerVehicle.direction.x);
	cam.x = ((cam.x - car.x) * cos(angle)) - ((cam.z - car.z) * sin(angle)) + car.x;
	cam.z = ((cam.x - car.x) * sin(angle)) + ((cam.z - car.z) * cos(angle)) + car.z;

	glm::mat4 viewMatrix = glm::lookAt(cam, car, up);
	return viewMatrix;
}


