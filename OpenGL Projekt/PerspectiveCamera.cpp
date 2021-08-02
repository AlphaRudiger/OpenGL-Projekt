#include "PerspectiveCamera.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#define PI 3.14159265358979323846f

PerspectiveCamera::PerspectiveCamera(float fov, float width, float height) : fov(fov), width(width), height(height), up(0, 1, 0), position(0, 0, 0), direction(0, 0, -1) {
	near = 1;
	far = 100;
	update();

}

void PerspectiveCamera::update() {
	float aspect = width / height;
	proj = glm::perspective(fov * (PI / 180), aspect, near, far);
	view = glm::lookAt(position, position + direction, up);
	combined = proj * view;
}
