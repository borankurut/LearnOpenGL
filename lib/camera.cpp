#include "camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
	
Camera::Camera(const glm::vec3& position, const glm::vec3& up, const glm::vec3& front){
	_position = position;
	_up = glm::normalize(up);
	_worldUp = _up;
	_front = front;
	updateVectors();
}

const glm::vec3& Camera::getPosition() const{return _position;}

const glm::vec3& Camera::getUp() const{return _up;}

const glm::vec3& Camera::getFront() const{return _front;}

glm::vec3 Camera::getTarget() const{
	return _position + _front;
}

// no need?
glm::vec3 Camera::getDirection() const{
	/* return glm::normalize(_position - getTarget()); //apparently -_front */
	return glm::normalize(-_front);
}

glm::vec3 Camera::getRight() const{
	return _right;
}

void Camera::setPosition(const glm::vec3& position){
	_position = position; 
}

void Camera::setFront(const glm::vec3& front){
	_front = front;
	updateVectors();
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

	setFront(glm::normalize(glm::vec3(x, y, z)));
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
	if(fov > MAX_FOV)
		fov = MAX_FOV;
	else if(fov < MIN_FOV)
		fov = MIN_FOV;

	_nearPlane.fov = fov;

	updateProjectionMatrix();
}

float Camera::getFov() const{
	return _nearPlane.fov;
}

const float Camera::getYaw() const {
	return _yaw;
}

const float Camera::getPitch() const{
	return _pitch;
}

void Camera::setPitch(float pitch){
	if (pitch > 89.0f)
        _pitch = 89.0f;
	else if (pitch < -89.0f)
        _pitch = -89.0f;
	else 
		_pitch = pitch;
}

void Camera::setYaw(float yaw){
	_yaw = yaw;
	if(_yaw > 180.0f)
		_yaw -= 360.0f;
	else if(_yaw < -180.0f)
		_yaw += 360.0f;
}

Camera* Camera::getCamera(GLFWwindow* window){
	WindowContext* context_p = static_cast<WindowContext*>(glfwGetWindowUserPointer(window));
	Camera * cam_p = context_p->camera;

	if(!cam_p){
		std::cerr << "getCamera can't find Camera\n";
	}

	return cam_p;
}


void Camera::handleMouseCallback(double pos_x, double pos_y){
	static float last_x = pos_x;
	static float last_y = pos_y;

	float x_off = pos_x - last_x;
	float y_off = -(pos_y - last_y); // y is reversed

	last_x = pos_x;
	last_y = pos_y;

	setYaw(_yaw + x_off * sensitivity);
	setPitch(_pitch + y_off * sensitivity);

	setFront(_yaw, _pitch);
}

void Camera::handleScrollCallback(double offset_x, double offset_y){
	float fov = _nearPlane.fov;

	setFov(_nearPlane.fov - offset_y);
}

void Camera::mouseCallback(GLFWwindow* window, double pos_x, double pos_y){
	Camera * cam = getCamera(window);
	if(!cam){
		std::cerr << "mouse callback can't get camera from window \n";
	}
	cam->handleMouseCallback(pos_x , pos_y);
}

void Camera::scrollCallback(GLFWwindow* window, double offset_x, double offset_y){
	Camera * cam = getCamera(window);
	if(!cam){
		std::cerr << "scroll callback can't get camera from window \n";
	}
	cam->handleScrollCallback(offset_x, offset_y);
}

void Camera::updateVectors(){
	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}

