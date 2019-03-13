#pragma once

#include <glm/glm.hpp>


class Entity
{
public:
	Entity();
	~Entity();

	glm::vec3 position;
	glm::mat4 transformationMatrix;

	float speed;				//Moved here do to issues confirming vehicle vs general entities
	float acceleration;



	int physicsIndex;
	int sceneObjectIndex;
	int gameStateIndex;
};

