#include "camera.hpp"
#include "glm/ext/matrix_transform.hpp"
	
Camera::Camera(const glm::vec3& position, const glm::vec3& up, const glm::vec3& front){
	_position = position;
	_up = glm::normalize(up);
	_front = front;
}


glm::vec3 Camera::getPosition() const{return _position;}

glm::vec3 Camera::getUp() const{return _up;}

glm::vec3 Camera::getTarget() const{
	return _position + _front;
}

glm::vec3 Camera::getDirection() const{
	return glm::normalize(_position - getTarget());
}
glm::vec3 Camera::getRight() const{
	return glm::normalize(glm::cross(_up, _front));
}

void Camera::setPosition(const glm::vec3& position){
	_position = position; 
}

void Camera::setFront(const glm::vec3& front){
	_front = front;
}

glm::mat4 Camera::getView() const{
	return glm::lookAt(_position, getTarget(), _up);
}
