// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

#include "shader.hpp"

// Variables
GLFWwindow* window;
const int width = 728, height = 728;

int main(void)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(width, height, "Square using EBO", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Specify the size of the rendering window
    glViewport(0, 0, width, height);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    //// Define vertices for the square (4 vertices)
    //GLfloat vertices[] = {
    //    // First triangle
    //    -0.75f, 0.0f, 0.0f,  // Vertex 0 (base left)
    //    -0.5f, 0.0f, 0.0f, // Vertex 1 (base right)
    //    -0.625f, 0.5f, 0.0f, // Vertex 2 (top point)

    //    // Second triangle
    //    -0.25f, 0.0f, 0.0f,  // Vertex 3 (base right)
    //    -0.375f, 0.5f, 0.0f, // Vertex 4 (top point)

    //    // Third triangle
    //    0.0f, 0.0f, 0.0f, // Vertex 5 (base right)
    //    -0.125f, 0.5f, 0.0f, // Vertex 6 (top point)

    //    // Fourth triangle
    //    0.25f, 0.0f, 0.0f,  // Vertex 7 (base right)
    //    0.125f, 0.5f, 0.0f  // Vertex 8 (top point)
    //};



    //GLuint indices[] = {
    //    // First triangle
    //    0, 1, 2,
    //    // Second triangle
    //    1, 3, 4,
    //    // Third triangle
    //    3, 5, 6,
    //    // Fourth triangle
    //    5, 7, 8
    //};



    // Smaller square vertices (shifted to the right by 0.75 and scaled down by 0.4)
    GLfloat vertices1[] = {
        0.75f + -0.2f, -0.2f, 0.0f,  // Bottom-left
        0.75f + 0.2f, -0.2f, 0.0f,  // Bottom-right
        0.75f + 0.2f,  0.2f, 0.0f,  // Top-right
        0.75f + -0.2f,  0.2f, 0.0f   // Top-left
    };

    // Square indices (2 triangles, 6 indices)
    GLuint indices1[] = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };

    // Triangle vertices (3 vertices)
    GLfloat vertices2[] = {
        -0.5f, -0.5f, 0.0f,  // Bottom-left
         0.5f, -0.5f, 0.0f,  // Bottom-right
         0.0f,  0.5f, 0.0f   // Top-center
    };

    // Triangle indices (1 triangle, 3 indices)
    GLuint indices2[] = {
        0, 1, 2  // Single triangle
    };



    
    // First VAO
    GLuint VAO1, VBO1, EBO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO1);

    // Bind VAO1 and configure VBO1 and EBO1
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
    glBindVertexArray(0);
    // Second VAO
    GLuint VAO2, VBO2, EBO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);

    // Bind VAO2 and configure VBO2 and EBO2
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
    glBindVertexArray(0);  // Unbind VAO2

    std::cout << "Bruh";
    // Rendering in the main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the first object (VAO1)
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, sizeof(indices1) / sizeof(indices1[0]), GL_UNSIGNED_INT, 0);

        // Draw the second object (VAO2)
       // glBindVertexArray(VAO2);
        //glDrawElements(GL_TRIANGLES, sizeof(indices2) / sizeof(indices2[0]), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // Cleanup VBO, EBO, VAO
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &EBO1);
    glDeleteBuffers(1, &VBO1);

    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &EBO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(programID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}