// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "ModelLoading\texture.h"

// Include Camera
#include "Camera\camera.h"


// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"

//variables
GLFWwindow* window;
int width = 1024, height = 768;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 30.0f);
glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera camera(cameraPos, cameraDir, cameraUp);

// time between current frame and last frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processKeyboardInput();
void onWindowCallback(GLFWwindow* window, int new_width, int new_height);

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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Load shader
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");


	//GLfloat vertices[] = {
	//	//we add coordinates for colors and textures
	//	//back
	//	// position			//colors			 //texture coord
	//	//front
	//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //0.0f, 0.0f,
	//	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //1.0f, 0.0f,
	//	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //1.0f, 1.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  //0.0f, 1.0f,

	//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f, //0.0f, 0.0f,
	//	0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 0.0f, //1.0f, 0.0f,
	//	0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f, //1.0f, 1.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f, //0.0f, 1.0f

	//};

	//For textures task
	
	GLfloat vertices[] = {
		//we add coordinates for colors and textures
		//back
		// position			//colors			 //texture coord
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 1.0f, 1.0f, 
		-0.5f,  0.5f, -0.5f,  1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 

		//front
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 
		-0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 

		 //left
		-0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 
		-0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 
		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 

		// right
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, 
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 

		 //bottom
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f, 1.0f, 
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 1.0f, 

		 //top
		-0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 
		-0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 
	};

	GLuint indices[] = {  
		0, 1, 2,  
		0, 2, 3, 
		2, 7, 3,
		2, 6, 7,
		6, 7, 4,
		6, 5, 4,
		0, 1, 4,
		1, 5, 4,
		2, 1, 6,
		6, 1, 5,
		3, 0, 4, 
		3, 4, 7
	};
	

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
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
	GLuint vbo, ibo, vao;
	//glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	//glGenBuffers(1, &ibo);

	//glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices position
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		8 * sizeof(float),     // stride
		0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	// 2nd attribute buffer : color coords
	glVertexAttribPointer(
		1,                  // attribute 1, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		8 * sizeof(float),                  // stride
		(void*)(3 * sizeof(float))            // array buffer offset
	);
	glEnableVertexAttribArray(1);

	// 3rd attribute buffer : texture coords
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(float),
		(void*)(6 * sizeof(float))
	);
	glEnableVertexAttribArray(2);

	//load texture
	GLuint texture1, texture2;
	texture1 = loadBMP("Images/wood.bmp");
	texture2 = loadBMP("Images/fire.bmp");

	//load a different texture, of your choice

	glfwSetFramebufferSizeCallback(window, onWindowCallback);

	// Check if the window was closed
	while (!glfwWindowShouldClose(window) && glfwGetKey(window,GLFW_KEY_ESCAPE)!=GLFW_PRESS)
	{
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();
		processKeyboardInput();

		// enable depth buffer
		// using the z buffer to establish which fragments are in front and should be drawn
		// or which ones should be discarded
		glEnable(GL_DEPTH_TEST); 

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float aspectRatio = (float)width / height;

		glm::mat4 model;

		//Adding camera
		glm::mat4 view = glm::lookAt(camera.getCameraPosition(),
			camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());

		glm::mat4 projection;
		projection = glm::perspective(90.0f, aspectRatio, 0.1f, 10000.0f);

		glm::mat4 MVP;

		// Use our shader for all objects
		glUseProgram(programID);

		//glBindVertexArray(vao);

		// render cubes
		for (unsigned int i = 0; i < 10; i++)
		{
			
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4(1.0);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, 10.0f * (i + 1) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));

			//update the MVP matrix
			MVP = projection * view * model;

			

			unsigned int transformLoc0 = glGetUniformLocation(programID, "model");
			glUniformMatrix4fv(transformLoc0, 1, GL_FALSE, glm::value_ptr(model));

			unsigned int transformLoc = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(MVP));

			unsigned int transformLoc2 = glGetUniformLocation(programID, "texture1");
			unsigned int transformLoc3 = glGetUniformLocation(programID, "texture2");
			unsigned int blendFactor = glGetUniformLocation(programID, "blendFactor");

			glBindTexture(GL_TEXTURE_2D, 0);

			if (i == 9) {

				glUniform1f(blendFactor, 0.5f);
				// Activate texture unit 0 and bind the first texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture1);
				
				glUniform1i(transformLoc2, 0);  // Texture unit 0

				// Activate texture unit 1 and bind the second texture
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, texture2);
				
				glUniform1i(transformLoc3, 1);  // Texture unit 1

			}
			else {
				glUniform1f(blendFactor, 0.0f);
				glActiveTexture(GL_TEXTURE0);


				if (i % 2 == 0) {
					glBindTexture(GL_TEXTURE_2D, texture1);
				}
				else {
					glBindTexture(GL_TEXTURE_2D, texture2);

				}

				glUniform1i(transformLoc2, 0);
				glUniform1i(transformLoc3, 1);  // Texture unit 1

			}


			

			//For ex 5

			//Bind the second texture
			//Make it also active

			//add the uniform for the second texture

			//For working with textures, replace with the appropriate glDrawArrays function call
			glDrawArrays(GL_QUADS, 0, 24);  // Use 36 because you have 6 faces with 2 triangles per face

		}

	} 
	
	// Cleanup VBO
	glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


//Process input for camera rotation
void processKeyboardInput()
{
	float cameraSpeed = deltaTime;

	//translation
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		camera.keyboardMoveFront(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		camera.keyboardMoveBack(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		camera.keyboardMoveLeft(cameraSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_D))
	{
		camera.keyboardMoveRight(cameraSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_R))
	{
		camera.keyboardMoveUp(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_F))
	{
		camera.keyboardMoveDown(cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_UP)) {
		camera.rotateOx(cameraSpeed * 50.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN)) {
		camera.rotateOx(cameraSpeed * -50.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
		camera.rotateOy(cameraSpeed * -50.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT)) {
		camera.rotateOy(cameraSpeed * 50.0f);
	}
}

//Resizing window triggers this callback and we recenter the viewport
void onWindowCallback(GLFWwindow* window, int new_width, int new_height) {

	glViewport(0, 0, new_width, new_height);

	width = new_width;
	height = new_height;
}