// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"

#define movements_increment 0.1f;


using namespace std;

//window variables
GLFWwindow* window;
const int width = 1024, height = 768;

// lighting
glm::vec3 lightPos(-3.0f, 0.0f, 3.0f);
glm::vec3 lightColor(1.0f);
bool isDragging = false;  // Track if the light source is being dragged
const float cubeDistanceFromCamera = 1.0f;

//View related vars:

// Initialize variables for camera rotation
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = width / 2.0;
float lastY = height / 2.0;
bool firstMouse = true;

glm::vec3 view_direction = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 observer_pos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 view;


//Projection
glm::mat4 projection;
const float far_plane = 10000.0f;
const float near_plane = 0.1f;
const float maxFOV = 140.0f;
const float minFOV = 0.1f;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		isDragging = !isDragging;  // Toggle dragging state
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	
	xOffset *= movements_increment;
	yOffset *= movements_increment;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	view_direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	view_direction.y = sin(glm::radians(pitch));
	view_direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	
	if (isDragging) {
		// Calculate the position of the cube in front of the camera
		lightPos = observer_pos + view_direction * cubeDistanceFromCamera;
	}
	

}


void window_callback(GLFWwindow* window, int new_width, int new_height)
{
	glViewport(0, 0, new_width, new_height);
}


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "3D demo", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Create and compile our GLSL program from the shaders
	// More about this in the following labs
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	GLuint lightSourceProgramID = LoadShaders("LightSource.vertexshader", "LightSourceFragmentShader.fragmentshader");

	float vertices[] = {
		// front
		 0.0, 0.0,  0.05,
		 0.05, 0.0,  0.05,
		 0.0,  0.05,  0.05,
		 0.05,  0.05,  0.05,
		 // back
		 0.0, 0.0, 0.0,
		 0.05, 0.0, 0.0,
		 0.0,  0.05, 0.0,
		 0.05,  0.05, 0.0
	};


	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2,
		1, 3, 2,
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4
	};

	glm::vec3 positions[] = {
		glm::vec3(0.0f,  0.0f,  0.2f),
		glm::vec3(0.2f,  0.5f, 0.1f),
		glm::vec3(-0.15f, -0.22f, 0),
		glm::vec3(-0.38f, -0.2f, -0.7),
		glm::vec3(0.24f, -0.4f, 0.1),
		glm::vec3(-0.17f,  0.3f, 0.7),
		glm::vec3(0.23f, -0.2f, 0.1),
		glm::vec3(0.15f,  0.2f, 0),
		glm::vec3(0.7f,  0.7f, 0.9),
		glm::vec3(-0.13f,  0.9f, 0)
	};


	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store 
	// the information for a complete rendered object.
	GLuint vbo, vao, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);
	
	glfwSetFramebufferSizeCallback(window, window_callback);



	

	float field_of_view = 45.0f;
	int width = 0, height = 0;

	

	//Mouse callbacks
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		
		//Changing view parameters (view directions + position)

		//Close window when pressing escape
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}

		//Compute relative axis (right and forward)
		glm::vec3 right_direction = glm::normalize(glm::cross(view_direction, up_vector));
		glm::vec3 forward_direction = glm::normalize(view_direction);

		// Use keys to move around
		int d_key = glfwGetKey(window, GLFW_KEY_D);
		int a_key = glfwGetKey(window, GLFW_KEY_A);

		// Right and left movement based on view_direction and up_vector cross product
		if (d_key == GLFW_PRESS && a_key != GLFW_PRESS) {
			// Move camera to the right (perpendicular to view_direction and up_vector)
			observer_pos += right_direction * movements_increment;
			cout << "Moving to the right: " << observer_pos.x << ", " << observer_pos.y << ", " << observer_pos.z << "\n";
		}
		else if (a_key == GLFW_PRESS && d_key != GLFW_PRESS) {
			// Move camera to the left (opposite direction)
			observer_pos -= right_direction * movements_increment;

			cout << "Moving to the left: " << observer_pos.x << ", " << observer_pos.y << ", " << observer_pos.z << "\n";
		}

		int w_key = glfwGetKey(window, GLFW_KEY_W);
		int s_key = glfwGetKey(window, GLFW_KEY_S);

		// Forward and backward movement based on view_direction
		if (w_key == GLFW_PRESS && s_key != GLFW_PRESS) {
			// Move camera forward
			observer_pos += forward_direction * movements_increment;
			cout << "Moving forward: " << observer_pos.x << ", " << observer_pos.y << ", " << observer_pos.z << "\n";
		}
		else if (s_key == GLFW_PRESS && w_key != GLFW_PRESS) {
			// Move camera backward
			observer_pos -= forward_direction * movements_increment;
			cout << "Moving backward: " << observer_pos.x << ", " << observer_pos.y << ", " << observer_pos.z << "\n";
		}

		int spacebar_key = glfwGetKey(window, GLFW_KEY_SPACE);
		int left_shift_key = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);

		// Up and down movement based on up_vector
		if (spacebar_key == GLFW_PRESS && left_shift_key != GLFW_PRESS) {
			// Move camera up
			
			observer_pos.y +=  movements_increment;
			
			cout << "Moving up: " << observer_pos.x << ", " << observer_pos.y << ", " << observer_pos.z << "\n";
		}
		else if (left_shift_key == GLFW_PRESS && spacebar_key != GLFW_PRESS) {
			// Move camera down
			
			observer_pos.y -= movements_increment;
			
			cout << "Moving down: " << observer_pos.x << ", " << observer_pos.y << ", " << observer_pos.z << "\n";
		}


		////Change view direction
		//int z_key = glfwGetKey(window, GLFW_KEY_Z);
		//int x_key = glfwGetKey(window, GLFW_KEY_X);
		//int y_key = glfwGetKey(window, GLFW_KEY_Y);


		//if (z_key == GLFW_PRESS && y_key != GLFW_PRESS && x_key != GLFW_PRESS) {
		//	
		//	//View Direction becomes z axis
		//	cout << "Changed view direction to Z axis" << "\n\n";

		//	view_direction = glm::vec3(0.0f, 0.0f, -1.0f);
		//	up_vector = glm::vec3(0.0f,1.0f,0.0f);

		//}
		//else if (y_key == GLFW_PRESS && z_key != GLFW_PRESS && x_key != GLFW_PRESS) {
		//
		//	//View direction becomes y axis
		//	cout << "Changed view direction to Y axis" << "\n\n";

		//	view_direction = glm::vec3(0.0f,1.0f,0.0f);
		//	up_vector = glm::vec3(0.0f, 0.0f, 1.0f);


		//}
		//else if (x_key == GLFW_PRESS && z_key != GLFW_PRESS && y_key != GLFW_PRESS) {
		//	
		//	//View direction becomes x axis
		//	cout << "Changed view direction to X axis" << "\n\n";
		//	view_direction = glm::vec3(1.0f, 0.0f, 0.0f);
		//	up_vector = glm::vec3(0.0f,1.0f,0.0f);
		//}

		//Change the perspective parameters:
		int v_key = glfwGetKey(window, GLFW_KEY_V);
		int b_key = glfwGetKey(window, GLFW_KEY_B);

		if (v_key == GLFW_PRESS && b_key != GLFW_PRESS) {

			//increase FOV
			cout << "Increasing FOV: " << field_of_view << "\n\n";
			if (field_of_view < maxFOV) {

				field_of_view += movements_increment;
			}
		}
		else if (b_key == GLFW_PRESS && v_key != GLFW_PRESS) {

			//decrease FOV
			cout << "Decreasing FOV: " << field_of_view << "\n\n";
			if (field_of_view > minFOV) {

				field_of_view -= movements_increment;
			}

		}

		//add view matrix
		view = glm::lookAt(observer_pos, view_direction + observer_pos, up_vector);
	
		//add projection matrix
		
		glfwGetWindowSize(window, &width, &height);

		float aspect_ratio = width / height;

		projection = glm::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
		
		
		for (int i = 0; i < 10; i++)
		{

			glm::mat4 model;
			
			model = glm::translate(model, positions[i]);

			//Leave 3 cubes static
			if (i != 0 && i != 4 && i != 8) {
				model = glm::rotate(model, (float)glfwGetTime() * 100, glm::vec3(0.0f, 1.0f, 0.0f));
			}

			//calculate MVP matrix

			unsigned int transformLoc = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(projection * view * model));


			unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
			glUniform4fv(transformLoc2, 1, glm::value_ptr(glm::vec4(0.5f + i / 10.0f, 1 - i / 10.0f, 0.5f, 1.0)));

			//bind VAO
			glBindVertexArray(vao);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		glUseProgram(lightSourceProgramID);

		// Set color for light source uniform
		glUniform3fv(glGetUniformLocation(lightSourceProgramID, "lightSourceColor"), 1, glm::value_ptr(lightColor));

		// Create model matrix for the cube's position
		glm::mat4 model = glm::translate(glm::mat4(1.0f), lightPos);
		glUniformMatrix4fv(glGetUniformLocation(lightSourceProgramID, "transform"), 1, GL_FALSE, glm::value_ptr(projection* view *model));

		

		// Render the cube
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	}

	// Cleanup VBO
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


