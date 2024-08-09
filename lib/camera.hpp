#pragma once

#include <glm/glm.hpp>

class Camera{
	
public:
	/*position, up, target*/
	Camera(const glm::vec3& position, const glm::vec3& up, const glm::vec3& target);

	glm::vec3 getPosition() const;
	glm::vec3 getUp() const;
	glm::vec3 getTarget() const;
	glm::vec3 getDirection() const;
	glm::vec3 getRight() const;

	void setPosition(const glm::vec3& position);
	void setFront(const glm::vec3& front);

	glm::mat4 getView() const;

private:
	glm::vec3 _position;
	glm::vec3 _up;
	glm::vec3 _front;
};

