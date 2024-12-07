#pragma once

#include <glm.hpp>
#include <gtx\transform.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "..\Graphics\window.h"

const float camera_sensitivity = 0.1f;


class Camera
{
	private:
		glm::vec3 cameraPosition;
		glm::vec3 cameraViewDirection;
		glm::vec3 cameraUp;
		glm::vec3 cameraRight;

		float yaw = -90.0f;
		float pitch = 0.0f;
		double lastMouseX;
		double lastMouseY;
		bool firstMove = true;

	public:
		Camera();
		Camera(glm::vec3 cameraPosition);
		Camera(glm::vec3 cameraPosition, glm::vec3 cameraViewDirection, glm::vec3 cameraUp);
		~Camera();
		glm::mat4 getViewMatrix();
		glm::vec3 getCameraPosition();
		glm::vec3 getCameraViewDirection();
		glm::vec3 getCameraUp();

		void keyboardMoveFront(float cameraSpeed);
		void keyboardMoveBack(float cameraSpeed);
		void keyboardMoveLeft(float cameraSpeed);
		void keyboardMoveRight(float cameraSpeed);
		void keyboardMoveUp(float cameraSpeed);
		void keyboardMoveDown(float cameraSpeed);

		void setCursorOrientation(double x, double y);

};

