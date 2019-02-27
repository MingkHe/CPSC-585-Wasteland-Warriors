#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Gamestate.h"

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

	glm::vec3 at = gameState->playerVehicle.position;
	glm::mat4 viewMatrix = glm::lookAt(pos, at, up);
	return viewMatrix;
}


