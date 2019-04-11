#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Explosion
{
public:
	Explosion();
	Explosion(glm::vec3 pos);
	~Explosion();

	int life = 0;
	glm::vec3 position;
};
