#include "camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
	
Camera::Camera(const glm::vec3& position, const glm::vec3& up, const glm::vec3& front){
	_position = position;
	_up = glm::normalize(up);
	_front = front;
}


const glm::vec3& Camera::getPosition() const{return _position;}

const glm::vec3& Camera::getUp() const{return _up;}

const glm::vec3& Camera::getFront() const{return _front;}

glm::vec3 Camera::getTarget() const{
	return _position + _front;
}

// no need?
glm::vec3 Camera::getDirection() const{
	return glm::normalize(_position - getTarget()); //apparently -_front
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

void Camera::setFront(float yaw_deg, float pitch_deg){
	float x, y, z, cos_yaw, sin_yaw, cos_pitch, sin_pitch;
	cos_yaw = glm::cos(glm::radians(yaw_deg));
	sin_yaw = glm::sin(glm::radians(yaw_deg));
	cos_pitch = glm::cos(glm::radians(pitch_deg));
	sin_pitch = glm::sin(glm::radians(pitch_deg));

	x = cos_pitch * cos_yaw;
	y = sin_pitch;
	z = cos_pitch * sin_yaw;

	_front = glm::normalize(glm::vec3(x, y, z));
}

glm::mat4 Camera::getView() const{
	return glm::lookAt(_position, getTarget(), _up);
}

glm::mat4 Camera::getProjection() const{
	return _projection_matrix;
}

void Camera::updateProjectionMatrix(){
	_projection_matrix = glm::perspective(glm::radians(_nearPlane.fov), _nearPlane.width / _nearPlane.height, _nearPlane.near_distance, _nearPlane.far_distance);
}

const Camera::NearPlane& Camera::getNearPlane() const{
	return _nearPlane;
}

void Camera::setNearPlane(const Camera::NearPlane& nearPlane){
	_nearPlane = nearPlane;
	updateProjectionMatrix();
}

void Camera::setFov(float fov){
	_nearPlane.fov = fov;
	updateProjectionMatrix();
}

float Camera::getFov() const{
	return _nearPlane.fov;
}

