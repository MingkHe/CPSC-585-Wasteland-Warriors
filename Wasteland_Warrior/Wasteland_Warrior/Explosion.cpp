#include "Explosion.h"

Explosion::Explosion()
{
}

Explosion::Explosion(glm::vec3 pos)
{
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;
}

Explosion::~Explosion()
{
}