#pragma once

#include <glm/glm.hpp>

class Entity
{
public:
	Entity();
	~Entity();

	glm::vec3 position;
	glm::mat4 transformationMatrix;
};

