#pragma once

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include "window_context.h"
#include <iostream>

class Camera{
	
public:
		struct NearPlane{
		float fov;
		float width;
		float height;
		float near_distance;
		float far_distance;
	};

	Camera(const glm::vec3& position, const glm::vec3& up, const glm::vec3& front);

	const glm::vec3& getPosition() const;
	const glm::vec3& getUp() const;
	const glm::vec3& getFront() const;

	const float getYaw() const;
	const float getPitch() const;

	void setYaw(float yaw);
	void setPitch(float pitch);

	glm::vec3 getTarget() const;
	glm::vec3 getDirection() const;
	glm::vec3 getRight() const;

	void setPosition(const glm::vec3& position);
	void setFront(const glm::vec3& front);

	void setFront(float yaw_deg, float pitch_deg);

	glm::mat4 getView() const;

	glm::mat4 getProjection() const;

	const NearPlane& getNearPlane() const;

	void setNearPlane(const NearPlane& nearPlane);

	const float MAX_FOV = 45.0f;
	const float MIN_FOV = 1.0f;

	float speed = 10.0f;

	float sensitivity = 0.1f;

	void setFov(float fov);

	float getFov() const;

	static void mouseCallback(GLFWwindow* window, double pos_x, double pos_y);

	static void scrollCallback(GLFWwindow* window, double offset_x, double offset_y);

private:
	glm::vec3 _position;
	glm::vec3 _up;
	glm::vec3 _front;
	glm::vec3 _right;
	glm::vec3 _worldUp;

	float _yaw;
	float _pitch;

	glm::mat4 _projection_matrix;

	NearPlane _nearPlane;

	void updateProjectionMatrix();

	static Camera* getCamera(GLFWwindow* window);

	void handleMouseCallback(double pos_x, double pos_y);

	void handleScrollCallback(double offset_x, double offset_y);

	void updateVectors();
};
