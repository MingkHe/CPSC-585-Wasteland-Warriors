#ifndef COMPOSITVEWORLDBJECT_H_
#define COMPOSITVEWORLDBJECT_H_

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Geometry.h"
#include "texture.h"

class CompositeWorldObject {
public:
	std::vector<Geometry> subObjects;
	glm::mat4 transform;
	std::vector<int> subobjectIndices;
};

#endif /* NODE_H_ */