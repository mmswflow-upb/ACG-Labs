// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <random>
using namespace std;
// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"

//variables
GLFWwindow* window;

const struct AppData {

	vector<glm::vec3> positions;
	vector<float> angles;
	int width, height;
	glm::vec3 bonusScaleVec3;
};


//Handling cursor position
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << "The mouse cursor is: " << xpos << " " << ypos << std::endl;
}

//Use this to generate random colors based on the given index (use index as seed for consistency)
glm::vec4 generate_random_color(int index) {
	// Seed the random number generator with the index
	std::mt19937 gen(index);  // Use the index as the seed for consistency
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);  // Uniform distribution between 0 and 1

	// Generate random values for red, green, and blue channels
	float r = dis(gen);
	float g = dis(gen);
	float b = dis(gen);

	// Return the color as a vec4 (RGBA), with alpha set to 1.0 for full opacity
	return glm::vec4(r, g, b, 1.0f);
}

//Bonus Ex
glm::vec3 generateRandomVec3() {
	// Define a random device and a random engine (Mersenne Twister in this case)
	std::random_device rd;  // Random device to seed the engine
	std::mt19937 gen(rd()); // Mersenne Twister engine seeded with random device

	// Define a distribution to generate random floats between 0.2 and 1.0
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	// Generate random scaling factors for x and y, keep z at 1.0f for a 2D square
	float x = dist(gen);
	float y = dist(gen);
	float z = 1.0f;  // Optional: You can randomize this too if working in 3D

	// Return the random scale vector
	return glm::vec3(x, y, z);
}

//Ex3 callback for the pressed middle mouse button  event
void middle_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {

		double xpos, ypos;
		 
		glfwGetCursorPos(window, &xpos, &ypos);

		AppData* app_data = static_cast<AppData*>(glfwGetWindowUserPointer(window));

		int width = app_data->width;
		int height = app_data->height;

		// Normalize cursor position to range [-1, 1]
		float x_normalized = ((2.0f * xpos) / width )- 1.0f;
		float y_normalized = 1.0f - ((2.0f * ypos) / height);

		app_data->positions.push_back(glm::vec3(x_normalized, y_normalized, 0.0f));
		app_data->angles.push_back(0.0f);

	}
}

//Ex4: Complete callback for adjusting the viewport when resizing the window
void window_callback(GLFWwindow* window, int new_width, int new_height)
{
	//what should we do here?
	glViewport(0, 0, new_width, new_height);
	AppData* app_data = static_cast<AppData*>(glfwGetWindowUserPointer(window));
	app_data->width = new_width;
	app_data->height = new_height;
}

//Bonus Ex: callback for pressing the key "e"
void bonus_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_E && action == GLFW_PRESS){
		
		AppData* app_data = static_cast<AppData*>(glfwGetWindowUserPointer(window));

		app_data->positions[0] = generateRandomVec3();
		glm::vec3 tempScaleVec3 = generateRandomVec3();
		tempScaleVec3.x = abs(tempScaleVec3.x);
		tempScaleVec3.y = abs(tempScaleVec3.y);

		app_data->bonusScaleVec3 = tempScaleVec3;

	}
}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	AppData app_data = { vector<glm::vec3>(), vector<float>(), 800, 800, glm::vec3(1.0f)};
	
	app_data.positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	app_data.angles.push_back(0.0f);

	window = glfwCreateWindow(app_data.width, app_data.height, "Lab 4", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Set the user pointer for the window
	glfwSetWindowUserPointer(window, &app_data);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, app_data.width, app_data.height);

	// Black background
	glClearColor(0.0f, 0.0f, 0.f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	//Bonus Ex, create and compile our extra shaders
	GLuint bonusProgramID = LoadShaders("BonusVertexShader.vertexshader", "BonusFragmentShader.fragmentshader");

	GLfloat vertices[] = {
		0.05f,  0.05f, 0.0f,  // top right
		0.05f, -0.05f, 0.0f,  // bottom right
		-0.05f, -0.05f, 0.0f,  // bottom left
		-0.05f,  0.05f, 0.0f   // top left 
	};

	GLuint indices[] = {  // note that we start from 0!
		0, 3, 1,  // first Triangle
		1, 3, 2,   // second Triangle
	};


	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object. 
	GLuint vbo, vao, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	//bind VAO
	glBindVertexArray(vao);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	//create matrices for transforms

	glm::mat4 transform(1.0f);
	glm::vec3 zAxis(0.0f, 0.0f, 1.0f);

	// Set a callback for handling mouse cursor position
	// Decomment for a callback example
	//glfwSetCursorPosCallback(window, cursor_position_callback);

	//Ex4 - Set callback for window resizing
	glfwSetFramebufferSizeCallback(window, window_callback);

	//Ex3 (Using a lambda function as a callback)
	glfwSetMouseButtonCallback(window, middle_mouse_button_callback	);

	//Bonus Ex
	glfwSetKeyCallback(window, bonus_key_callback);

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

		//Ex1
		int escape_state = glfwGetKey(window, GLFW_KEY_ESCAPE);
		if (escape_state == GLFW_PRESS)
		{
			break;
		}
		

		//Ex2
		int right_click_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		int left_click_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		if (right_click_state == GLFW_PRESS && left_click_state != GLFW_PRESS) {
			for (int i = 0; i < app_data.angles.size(); i++) {

				app_data.angles.at(i) += 1.0f;
			}
		}
		if (left_click_state == GLFW_PRESS && right_click_state != GLFW_PRESS) {
			for (int i = 0; i < app_data.angles.size(); i++) {
				app_data.angles.at(i) -= 1.0f;
			}
		}

		//All squares after the first one will be drawn using simplevertex and simplefragment shaders
		for (int i = 1; i < app_data.positions.size(); i++) {

			//reset transform matrix on each iteration
			transform = glm::mat4(1.0f);

			//Assign transform to move to certain position
			transform = glm::translate(transform, app_data.positions.at(i));
			

			//ORDER MATTERS

			//Rotate around origin
			transform = glm::rotate(transform, app_data.angles.at(i), zAxis);

			// send variables to shader
			unsigned int transformLoc = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

			unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
			glm::vec4 color = generate_random_color(i);
			glUniform4fv(transformLoc2, 1, glm::value_ptr(color));

			//bind VAO
			glBindVertexArray(vao);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		//Bonus Exfirst square will be rendered using the extra shaders
		glUseProgram(bonusProgramID);
		
		//Reset transform matrix
		transform = glm::mat4(1.0f);
		//Scale by random vector
		transform = glm::scale(transform, app_data.bonusScaleVec3);
		//Move to a random position
		transform = glm::translate(transform, app_data.positions[0]);

		//Pas transform to extra vertex shader
		unsigned int bonusTransformLoc = glGetUniformLocation(bonusProgramID, "bonusTransform");
		glUniformMatrix4fv(bonusTransformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		//Pass random color to extra frag shader
		unsigned int bonusColorLoc = glGetUniformLocation(bonusProgramID, "bonusColor");
		glm::vec4 color = generate_random_color(0);
		glUniform4fv(bonusColorLoc, 1, glm::value_ptr(color));

		//bind VAO
		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	// Cleanup
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}