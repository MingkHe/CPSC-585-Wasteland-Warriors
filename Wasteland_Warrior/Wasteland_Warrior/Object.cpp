#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
}

Object::Object(int type, float x, float y)
{
	this->type = type;
	this->position = glm::vec3(x, y, 0.0);
}