#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Gamestate.h"

using namespace std;
using namespace glm;

const float MAX_VERT = 0.99f;

/*
Camera::Camera(Gamestate* newGamestate) {
	gameState = newGamestate;
}*/

glm::mat4 Camera::viewMatrix() const {
	//int test = gameState->gstest;
	glm::vec3 at = gameState->playerVehicle.position;
	//glm::vec3 at = { 0.0f, 0.0f, 20.0f };
	glm::mat4 viewMatrix = glm::lookAt(pos, at, up);
	return viewMatrix;
}

void Camera::setGamestate(Gamestate* newGamestate) {
	gameState = newGamestate;
}

