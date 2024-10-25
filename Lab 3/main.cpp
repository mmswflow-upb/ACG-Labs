// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

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
const int width = 1024, height = 1024;
float sx = 1.5f, sy = 0.5, sz = 0;


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
	window = glfwCreateWindow(width, height, "Lab 3 (CUBE ACHIEVED)", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		getchar();
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);


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

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	float g_vertex_buffer_data[] = {
		0.05f,  0.05f, 0.0f,  // top right
		0.05f, -0.05f, 0.0f,  // bottom right
		-0.05f, -0.05f, 0.0f,  // bottom left
		-0.05f,  0.05f, 0.0f   // top left 
	};



	unsigned int indices[] = {  // note that we start from 0!
		0, 3, 1,  // first Triangle
		1, 3, 2,   // second Triangle
	};


	// Positions for 10 squares
	glm::vec3 positions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.2f, 0.5f, 0.0f),
		glm::vec3(-0.15f, -0.22f, 0.0f),
		glm::vec3(-0.38f, -0.2f, 0.0f),
		glm::vec3(0.24f, -0.4f, 0.0f),
		glm::vec3(-0.17f, 0.3f, 0.0f),
		glm::vec3(0.93f, -0.2f, 0.0f),
		glm::vec3(0.15f, 0.2f, 0.0f),
		glm::vec3(0.15f, 0.7f, 0.0f),
		glm::vec3(-0.13f, 0.1f, 0.0f)
	};




	float cube_vertices[] = {
		// Positions          // Colors (R, G, B)
		-0.05, -0.05, -0.05,  1.0f, 0.0f, 0.0f,  // Back face (Red)
		 0.05, -0.05, -0.05,  1.0f, 0.0f, 0.0f,
		 0.05,  0.05, -0.05,  1.0f, 0.0f, 0.0f,
		-0.05,  0.05, -0.05,  1.0f, 0.0f, 0.0f,

		-0.05, -0.05,  0.05,  0.0f, 1.0f, 0.0f,  // Front face (Green)
		 0.05, -0.05,  0.05,  0.0f, 1.0f, 0.0f,
		 0.05,  0.05,  0.05,  0.0f, 1.0f, 0.0f,
		-0.05,  0.05,  0.05,  0.0f, 1.0f, 0.0f,

		-0.05,  0.05,  0.05,  0.0f, 0.0f, 1.0f,  // Left face (Blue)
		-0.05,  0.05, -0.05,  0.0f, 0.0f, 1.0f,
		-0.05, -0.05, -0.05,  0.0f, 0.0f, 1.0f,
		-0.05, -0.05,  0.05,  0.0f, 0.0f, 1.0f,

		 0.05,  0.05,  0.05,  1.0f, 1.0f, 0.0f,  // Right face (Yellow)
		 0.05,  0.05, -0.05,  1.0f, 1.0f, 0.0f,
		 0.05, -0.05, -0.05,  1.0f, 1.0f, 0.0f,
		 0.05, -0.05,  0.05,  1.0f, 1.0f, 0.0f,

		-0.05, -0.05, -0.05,  1.0f, 0.0f, 1.0f,  // Bottom face (Magenta)
		 0.05, -0.05, -0.05,  1.0f, 0.0f, 1.0f,
		 0.05, -0.05,  0.05,  1.0f, 0.0f, 1.0f,
		-0.05, -0.05,  0.05,  1.0f, 0.0f, 1.0f,

		-0.05,  0.05, -0.05,  0.0f, 1.0f, 1.0f,  // Top face (Cyan)
		 0.05,  0.05, -0.05,  0.0f, 1.0f, 1.0f,
		 0.05,  0.05,  0.05,  0.0f, 1.0f, 1.0f,
		-0.05,  0.05,  0.05,  0.0f, 1.0f, 1.0f
	};

	unsigned int cube_indices[] = {
		0, 1, 2, 2, 3, 0,   // Back face
		4, 5, 6, 6, 7, 4,   // Front face
		8, 9, 10, 10, 11, 8, // Left face
		12, 13, 14, 14, 15, 12, // Right face
		16, 17, 18, 18, 19, 16, // Bottom face
		20, 21, 22, 22, 23, 20  // Top face
	};

	glm::vec3 cube_positions[10] = {
	glm::vec3(-0.2f, 0.2f, 0.0f),   // Cube 1 (top-left)
	glm::vec3(0.0f, 0.2f, 0.0f),    // Cube 2 (top-center)
	glm::vec3(0.2f, 0.2f, 0.0f),    // Cube 3 (top-right)

	glm::vec3(-0.7f, 0.0f, 0.0f),   // Cube 4 (middle-left)
	glm::vec3(0.8f, 0.0f, 0.0f),    // Cube 5 (middle-center)
	glm::vec3(0.2f, 0.0f, 0.0f),    // Cube 6 (middle-right)

	glm::vec3(-0.4f, -0.2f, 0.0f),  // Cube 7 (bottom-left)
	glm::vec3(0.0f, 0.2f, 0.0f),   // Cube 8 (bottom-center)
	glm::vec3(0.5f, -0.2f, 0.0f),   // Cube 9 (bottom-right)

	glm::vec3(0.4f, -0.2f, 0.0f)    // Cube 10 (extra cube on the right side)
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
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	////set attribute pointers
	//glVertexAttribPointer(
	//	0,                  // attribute 0, must match the layout in the shader.
	//	3,                  // size of each attribute
	//	GL_FLOAT,           // type
	//	GL_FALSE,           // normalized?
	//	3 * sizeof(float),  // stride
	//	(void*)0            // array buffer offset
	//);
	//glEnableVertexAttribArray(0);

	// Position attribute (location = 0)
	glVertexAttribPointer(
		0,                  // Attribute 0 (position)
		3,                  // Number of components per vertex (x, y, z)
		GL_FLOAT,           // Type
		GL_FALSE,           // Normalize
		6 * sizeof(float),  // Stride (6 floats per vertex: 3 for position + 3 for color)
		(void*)0            // Offset to position data (starts at the beginning)
	);
	glEnableVertexAttribArray(0);

	// Color attribute (location = 1)
	glVertexAttribPointer(
		1,                  // Attribute 1 (color)
		3,                  // Number of components per color (r, g, b)
		GL_FLOAT,           // Type
		GL_FALSE,           // Normalize
		6 * sizeof(float),  // Stride (6 floats per vertex: 3 for position + 3 for color)
		(void*)(3 * sizeof(float)) // Offset to color data (starts after position data)
	);
	glEnableVertexAttribArray(1);


	

	

	
	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{

		//create matrices for transforms
		glm::mat4 translate(1.0f);
		glm::mat4 rot(1.0f);

		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		//Clear z buffer (Depth)
		GLint depthFuncValue;
		glGetIntegerv(GL_DEPTH_FUNC, &depthFuncValue);
		glClearDepth(depthFuncValue == GL_LESS ? 1.0f : 0.0f);
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);


		float time = (float) glfwGetTime();
		
		//Set color of square
		//glm::vec4 color;
		//color.x = abs(sin(time + 3.14));
		//color.y = abs(sin(2 * time + 6.28));
		//color.z = abs(sin(3 * time + 9.42));
		//color[3] = 1.0f;
		//unsigned int colorLoc = glGetUniformLocation(programID, "vertexColor");
		//glUniform4fv(colorLoc, 1, glm::value_ptr(color));



		//for (int i = 0; i < 10; i++) {
		//	// Set the color for each square
		//	glm::vec4 color;
		//	color.x = abs(sin(time + 3.14));
		//	color.y = abs(sin(i *2 * time + 6.28));
		//	color.z = abs(sin(i* 3 * time + 9.42));
		//	color[3] = 1.0f;
		//	unsigned int colorLoc = glGetUniformLocation(programID, "vertexColor");
		//	glUniform4fv(colorLoc, 1, glm::value_ptr(color));

		//	// Create the transformation matrix for each square
		//	translate = glm::translate(translate, positions[i]);  // Apply translation

		//	// Send transform matrix to the shader
		//	unsigned int transformLoc = glGetUniformLocation(programID, "transform");
		//	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(translate));

		//	// Bind VAO and draw the triangles
		//	glBindVertexArray(vao);
		//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//}

		for (int i = 0; i < 10; i++) {

			//translate = glm::scale(translate, glm::vec3(sx, sy, sz));
			unsigned int rotLoc = glGetUniformLocation(programID, "rotation");
			unsigned int transformLoc = glGetUniformLocation(programID, "transform");

			//inverted
			float inverted = powf(-1,i);

			// send variables to shader
			rot = glm::rotate(rot, time * 100, glm::vec3( sin(i* inverted * time),  cos(inverted * time), sin(5 * inverted * time)));// Around origin (so z axis)
			glUniformMatrix4fv(rotLoc, 1, GL_FALSE, glm::value_ptr(rot));


			translate = glm::translate(translate, cube_positions[i]);
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(translate));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); //6 squares from all sides, each square has two triangles, each triangle requires 3 vertices

		}



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


