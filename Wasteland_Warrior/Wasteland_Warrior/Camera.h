#include <glm/glm.hpp>
//#include "Gamestate.h"

class Camera {
public:
	glm::vec3 dir, right, up, pos;
	float radius;

	Camera() :dir(glm::vec3(0, 0, -1)), right(glm::vec3(1, 0, 0)), up(glm::vec3(0, 1, 0)), pos(glm::vec3(0)), radius(1.f) {}
	//Gamestate* newGameState

	glm::mat4 viewMatrix() const;

	//Gamestate* gameState;
};
