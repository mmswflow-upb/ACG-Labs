#include "camera.h"

Camera::Camera(glm::vec3 cameraPosition)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->right = glm::cross(cameraViewDirection, cameraUp);
}

Camera::Camera()
{
	this->cameraPosition = glm::vec3(0.0f, 0.0f, 100.0f);
	this->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->right = glm::cross(cameraViewDirection, cameraUp);
}

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraViewDirection, glm::vec3 cameraUp)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = cameraViewDirection;
	this->cameraUp = cameraUp;
	this->right = glm::cross(cameraViewDirection, cameraUp);
}

Camera::~Camera()
{
}

void Camera::keyboardMoveFront(float cameraSpeed)
{
	cameraPosition += cameraViewDirection * cameraSpeed * 50.0f;
}

void Camera::keyboardMoveBack(float cameraSpeed)
{
	cameraPosition -= cameraViewDirection * cameraSpeed *  50.0f;
}

void Camera::keyboardMoveLeft(float cameraSpeed)
{
	cameraPosition -= right * cameraSpeed * 50.0f;
}

void Camera::keyboardMoveRight(float cameraSpeed)
{
	cameraPosition += right * cameraSpeed * 50.0f;

}

void Camera::keyboardMoveUp(float cameraSpeed)
{
	cameraPosition += cameraUp * cameraSpeed * 10.0f;
}

void Camera::keyboardMoveDown(float cameraSpeed)
{
	cameraPosition -= cameraUp * cameraSpeed * 10.0f;
}

void Camera::rotateOx(float angle)
{
	cameraViewDirection = glm::normalize(glm::vec3((glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(cameraViewDirection, 1))));

	cameraUp = glm::normalize(glm::cross(right, cameraViewDirection));
}

void Camera::rotateOy (float angle)
{
	cameraViewDirection = glm::normalize(glm::vec3((glm::rotate(glm::mat4(1.0f), angle, cameraUp) * glm::vec4(cameraViewDirection, 1))));
	right = glm::cross(cameraViewDirection, cameraUp);
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


