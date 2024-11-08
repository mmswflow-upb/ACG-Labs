#include "camera.h"


Camera::Camera()
{
	this->cameraPosition = glm::vec3(0.0f, 30.0f, 100.0f);
	this->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
}


Camera::Camera(glm::vec3 cameraPosition)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraViewDirection, glm::vec3 cameraUp)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = cameraViewDirection;
	this->cameraUp = cameraUp;
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

Camera::~Camera()
{
}

void Camera::rotateOx(float angle)
{
	//Around right direction
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, cameraRight);
	glm::vec4 viewDirection4(cameraViewDirection, 1.0f);

	viewDirection4 = rot * viewDirection4;

	cameraViewDirection = glm::normalize(glm::vec3(viewDirection4.x, viewDirection4.y, viewDirection4.z));
	cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

void Camera::rotateOy (float angle)
{
	//around up direction
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle , cameraUp);
	glm::vec4 viewDirection4(cameraViewDirection, 1.0f);

	viewDirection4 = rot * viewDirection4;

	cameraViewDirection = glm::normalize(glm::vec3(viewDirection4.x, viewDirection4.y, viewDirection4.z));
	cameraUp = glm::cross(cameraRight, cameraViewDirection);
}

void Camera::translateUp(float speed)
{
	cameraPosition += cameraUp * speed;
}

void Camera::translateDown(float speed)
{
	cameraPosition -= cameraUp * speed;
}

void Camera::translateFront(float speed)
{
	//Move forward
	cameraPosition += cameraViewDirection * speed;
}

void Camera::translateBack(float speed)
{
	//Backwards
	cameraPosition -= cameraViewDirection * speed;
}


glm::vec3 Camera::getCameraPosition()
{
	return cameraPosition;
}

glm::vec3 Camera::getCameraViewDirection()
{
	return cameraViewDirection;
}

glm::vec3 Camera::getCameraUp()
{
	return cameraUp;
}

void Camera::setCameraPosition(glm::vec3 cameraPosition)
{
	this->cameraPosition = cameraPosition;
}

void Camera::setCameraUp(glm::vec3 cameraUp)
{
	this->cameraUp = cameraUp;
}

void Camera::setCameraViewDirection(glm::vec3 cameraViewDirection)
{
	this->cameraViewDirection = cameraViewDirection;
}

