#include "PowerUp.h"

PowerUp::PowerUp()
{
}

PowerUp::~PowerUp()
{
}

PowerUp::PowerUp(int type, float x, float y)
{
	this->type = type;
	this->position = glm::vec3(x, y, 0.0);
}