// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include Camera
#include "Camera\camera.h"

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
const int width = 1024, height = 768;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 30.0f);
glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera camera(cameraPos, cameraDir, cameraUp);

// time between current frame and last frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(.3f, .4f, .5f);
glm::vec3 lightColor(1.0);

void processKeyboardInput();

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
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Lighting", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Load shader
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	GLuint programIDLight = LoadShaders("LightVertexShader.vertexshader", "LightFragmentShader.fragmentshader");

	GLfloat vertices[] = {
		//we add coordinates for NORMALS 
		//back
		// position			//normals		
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  //0
		0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  //1
		0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  //2
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  //3

		//front
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  //4
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  //5
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  //6
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //7

		 //left
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, //8
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, //9
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, //10
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, //11

		// right
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //12
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, //13
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, //14
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //15

		 //bottom
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, //16
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, //17
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, //18
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,//19

		 //top
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, //20
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, //21
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //22
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  //23
	};
	

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  1.0f,  -3.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object. 
	GLuint vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices position
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		6 * sizeof(float),     // stride
		0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	// 2nd attribute buffer : normal coords
	glVertexAttribPointer(
		1,                  // attribute 1, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		6 * sizeof(float),                  // stride
		(void*)(3 * sizeof(float))            // array buffer offset
	);
	glEnableVertexAttribArray(1);
	
	glfwSetFramebufferSizeCallback(window, window_callback);

	// Check if the window was closed
	while (!glfwWindowShouldClose(window) && glfwGetKey(window,GLFW_KEY_ESCAPE)!=GLFW_PRESS)
	{
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();
		processKeyboardInput();

		//enable depth buffer
		glEnable(GL_DEPTH_TEST);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		lightPos.x = 1.0f + sin(currentFrame) * 3.0f;
		lightPos.z = sin(currentFrame / 2.0f) * 1.0f;

		glm::mat4 model;

		//Adding camera
		glm::mat4 view = glm::lookAt(camera.getCameraPosition(),
			camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());

		glm::mat4 projection;
		projection = glm::perspective(40.0f, (float)width / height, 0.1f, 10000.0f);

		glm::mat4 MVP;

		// Use our shader for all objects
		glUseProgram(programID);

		glBindVertexArray(vao);


		unsigned int lightPosLoc = glGetUniformLocation(programID, "lightPos");
		glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

		unsigned int viewPosLoc = glGetUniformLocation(programID, "viewPos");
		glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera.getCameraPosition()));

		// render cubes
		for (unsigned int i = 0; i < 10; i++)
		{
			
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4(1.0);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, 3.0f * i, glm::vec3(1.0f, 0.3f, 0.5f));

			//update the MVP matrix
			MVP = projection * view * model;

			unsigned int transformLoc0 = glGetUniformLocation(programID, "model");
			glUniformMatrix4fv(transformLoc0, 1, GL_FALSE, glm::value_ptr(model));

			unsigned int transformLoc = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(MVP));

			

			glDrawArrays(GL_QUADS, 0, 24);
		}

		//light
		unsigned int transformLoc4 = glGetUniformLocation(programID, "lightColor");
		glUniform3fv(transformLoc4, 1, glm::value_ptr(lightColor));
		
				
		//TODO: send light position and viewer position to the shader
		//you need them for diffuse and specular lighting


		//use the shader for the light
		glUseProgram(programIDLight);
		

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.5f)); // a smaller cube
		MVP = projection * view * model;

		unsigned int transformLoc = glGetUniformLocation(programIDLight, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(MVP));

		unsigned int transformLoc2 = glGetUniformLocation(programIDLight, "lightColor");
		glUniform4fv(transformLoc2, 1, glm::value_ptr(lightColor));

		glDrawArrays(GL_QUADS, 0, 24);

	} 
	
	// Cleanup VBO
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


//Process input for camera rotation
void processKeyboardInput()
{
	float cameraSpeed = deltaTime*2.2;

	//translation
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		camera.keyboardMoveFront(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		camera.keyboardMoveBack(cameraSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		camera.keyboardMoveUp(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{
		camera.keyboardMoveDown(cameraSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_UP))
	{
		camera.rotateOx(cameraSpeed/2);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN))
	{
		camera.rotateOx(-cameraSpeed/2);
	}

}





