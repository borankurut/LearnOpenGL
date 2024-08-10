#pragma once

#include <glm/glm.hpp>

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

	void setFov(float fov);

	float getFov() const;


private:
	glm::vec3 _position;
	glm::vec3 _up;
	glm::vec3 _front;

	glm::mat4 _projection_matrix;

	NearPlane _nearPlane;

	void updateProjectionMatrix();
};

