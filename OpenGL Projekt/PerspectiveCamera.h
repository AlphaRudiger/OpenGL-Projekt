#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class PerspectiveCamera {
public:
	float fov;
	float width, height;
	float near, far;
	glm::mat4 proj, view;
	glm::mat4 combined;
	glm::vec3 direction, up;
public:
	glm::vec3 position;

public:

	PerspectiveCamera(float fov, float width, float height);

	void update();

};