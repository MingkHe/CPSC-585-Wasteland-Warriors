#ifndef COMPOSITVEWORLDBJECT_H_
#define COMPOSITVEWORLDBJECT_H_

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Geometry.h"
#include "texture.h"
using namespace std;

class CompositeWorldObject {
public:
	std::vector<Geometry> subObjects;
	std::vector<int> textureObjectIndexMap;
	std::vector<string> texturePaths;
	glm::mat4 transform;
	std::vector<int> subobjectIndices;
	int subObjectsCount = 0;
	float transparent = 1.f;
	std::vector<glm::vec3> vertsPhys;
};

#endif /* NODE_H_ */