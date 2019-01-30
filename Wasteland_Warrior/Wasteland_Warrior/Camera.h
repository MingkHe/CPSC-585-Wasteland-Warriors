#include <glm/glm.hpp>

class Camera {
public:
	glm::vec3 dir, right, up, pos;
	float radius;

	Camera() :dir(glm::vec3(0, 0, -1)), right(glm::vec3(1, 0, 0)), up(glm::vec3(0, 1, 0)), pos(glm::vec3(0)), radius(1.f) {}
	Camera(float radius) :dir(glm::vec3(0, 0, -1)), right(glm::vec3(1, 0, 0)), up(glm::vec3(0, 1, 0)), pos(glm::vec3(0)), radius(radius) {}
	Camera(glm::vec3 dir, glm::vec3 right, glm::vec3 up, glm::vec3 pos, float radius) :dir(dir), right(right), up(up), pos(pos), radius(radius) {}


	glm::mat4 viewMatrix() const;

	void rotateVertical(float radians);
	void rotateHorizontal(float radians);
	void move(glm::vec3 movement);		//Moves in rotated frame
};
