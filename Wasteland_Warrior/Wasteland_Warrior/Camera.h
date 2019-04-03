#pragma once
#include <glm/glm.hpp>

class Gamestate;

class Camera {
public:
	glm::vec3 dir, right, up, pos;
	float radius;

	Camera(Gamestate* newGamestate);

	glm::mat4 viewMatrix() const;
	glm::mat4 backviewMatrix() const;

	Gamestate* gameState;
};