#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
 
#include "shader.h"

// Set window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
GLFWwindow* MAIN_WINDOW;

GLuint VAO, VBO, shaderProgram;

void createTriangle() {
    GLfloat vertices[] = {
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    };

    // Create VAO and set it to current buffer context
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Reserves memory for the array
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure vertex shader program attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

    // Unbind buffer from context
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind vertex array
    glBindVertexArray(0);
}

int initGLFW() {
	// Initialize GLFW
	if (!glfwInit()) { // throws error if failed
		printf("GLFW initialization failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// Set OpenGL version to be used (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	MAIN_WINDOW = glfwCreateWindow(WIDTH, HEIGHT, "processamento-grafico", NULL, NULL);
	if (!MAIN_WINDOW) { // throws error if failed
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information from the created window
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(MAIN_WINDOW, &bufferWidth, &bufferHeight);

	// Set the context for GLEW to use
	glfwMakeContextCurrent(MAIN_WINDOW);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW initialization failed");
		glfwDestroyWindow(MAIN_WINDOW);
		glfwTerminate();
		return 1;
	}

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);
}

int main() {

	if (!initGLFW()) {
		printf("Failed to initialize GLFW window");
		return 1;
	}

	// Creates a VAO holding triangle data
	createTriangle();

	// Creates and sets the shader program id to a global variable
	Shader ourShader("shaders/shader.vs", "shaders/shader.fs");

	// Loop until window closed
	while (!glfwWindowShouldClose(MAIN_WINDOW)) {
		// Get and handle user input events
		glfwPollEvents();

		// Clear the window
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Clear only the color (there is more data that can be cleared --like depth)
		glClear(GL_COLOR_BUFFER_BIT);

        // Set current shader program
		ourShader.use();
        // Set current vertex array
        glBindVertexArray(VAO);

        // (what we want to draw, array offset, array stride --#vertices to read)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Unassign vertex array;
        glBindVertexArray(0);
        // Unassign shader after using it
        glUseProgram(0);

		glfwSwapBuffers(MAIN_WINDOW);
	}

	return 0;
}