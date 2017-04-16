#include "Camera.h"

Camera::Camera() {
	pos = glm::vec3(0.0f, 0.0f, -5.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	projPerspective = glm::perspective(45.0f, 1080.0f / 768.0f, 0.01f, 1000.0f);
	projOrthographic = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.01f, 1000.0f);
}

Camera::~Camera() {
}

glm::mat4 Camera::GetView(void) {
	return glm::toMat4(orientation) * glm::translate(pos);
}

glm::mat4 Camera::GetProjection(bool ortho) {
	return ortho ? projOrthographic : projPerspective;
}

void Camera::SetPosition(glm::vec3 pos) {
	this->pos = pos;
}

void Camera::SetTarget(glm::vec3 target) {
	this->target = target;
	orientation = glm::quat(target - pos);
}

void Camera::SetUp(glm::vec3 up) {
	this->up = up;
}

void Camera::MoveForward(float increment) {
	pos.z += increment;
}

void Camera::MoveSideways(float increment) {
	pos.x += increment;
}

void Camera::MoveVertical(float increment) {
	pos.y += increment;
}

void Camera::ChangePitch(float increment) {
	orientation = glm::angleAxis(increment, glm::cross(up, pos - target)) * (orientation);
}

void Camera::ChangeRoll(float increment) {
	orientation = glm::angleAxis(increment, pos - target) * (orientation);
}

void Camera::ChangeYaw(float increment) {
	orientation = glm::angleAxis(increment, up) * (orientation);
}
