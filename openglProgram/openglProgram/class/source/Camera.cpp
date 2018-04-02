
#include <iostream>
#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

Camera::Camera(glm::vec3 position,glm::vec3 front):
	yaw(YAW),
	pitch(PITCH),
	zoom(ZOOM),
	mouseSensitivity(SENSITIVITY),
	movementSpeed(SPEED),
	worldUp(glm::vec3(0.0, 1.0, 0.0))
{
	cameraFront = front;
	cameraPosition = position;
	updateCameraDir();
}

void Camera::updateCameraDir()
{
	glm::vec3 front;

	front.x = -glm::sin(glm::radians(yaw));
	front.y = glm::sin(glm::radians(pitch));
	front.z = -glm::cos(glm::radians(pitch))*glm::cos(glm::radians(yaw));

	cameraFront = glm::normalize(front);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::processKeyboardInput(Camera_Movement moveDir, GLfloat deltaTime)
{
	GLfloat speed = movementSpeed * deltaTime;
	if (moveDir == Camera_Movement::UP)
		cameraPosition += cameraUp * speed;
	if (moveDir == Camera_Movement::DOWN)
		cameraPosition -= cameraUp * speed;
	if (moveDir == Camera_Movement::LEFT)
		cameraPosition -= cameraRight * speed;
	if (moveDir == Camera_Movement::RIGHT)
		cameraPosition += cameraRight * speed;
}

void Camera::processWheel(double offy)
{
	GLfloat speed = 0.3f;
	std::cout << offy << std::endl;
	cameraPosition += cameraFront * (GLfloat)offy * speed;
}

void Camera::processCursor(GLdouble offx, GLdouble offy)
{
	offx *= mouseSensitivity;
	offy *= mouseSensitivity;

	this->pitch += offy;
	this->yaw += offx;

	if (pitch >= 89)
		pitch = 89;
	if (pitch <= -89)
		pitch = -89;

	updateCameraDir();
}

glm::mat4 Camera::getCamearMatrix()
{
	return glm::lookAt(cameraPosition, cameraPosition + cameraFront, worldUp);
}