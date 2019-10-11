#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Set window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shaderProgram;

// Vertex shader program
static const char* vertexShader = "                                           \n\
#version 410                                                                  \n\
                                                                              \n\
layout (location = 0) in vec2 pos;											  \n\
layout (location = 1) in vec3 color_in;										  \n\
																			  \n\
out vec3 color_frag;														  \n\
                                                                              \n\
void main() {                                                                 \n\
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);							      \n\
	color_frag = color_in;													  \n\
}";
// Fragment shader program
static const char* fragmentShader = "                                         \n\
#version 410                                                                  \n\
                                                                              \n\
in vec3 color_frag;															  \n\
out vec4 color_out;                                                           \n\
                                                                              \n\
void main() {                                                                 \n\
    color_out = vec4(color_frag, 1.0);                                        \n\
}";

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

void addShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);

    const GLchar* GLcode[1];
    GLcode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    // (shader, amount of strings to join in one code, code itself, length)
    glShaderSource(shader, 1, GLcode, codeLength);
    glCompileShader(shader);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    // Throw error if compilation fails
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        printf("Error compiling the %d shader program: '%s' \n", shaderType, errorLog);
        return;
    }

    glAttachShader(shaderProgram, shader);
}

void compileShaders() {
    shaderProgram = glCreateProgram();
    if (!shaderProgram) {
        printf("Error creating shader program!\n");
        return;
    }

    addShader(shaderProgram, vertexShader, GL_VERTEX_SHADER);
    addShader(shaderProgram, fragmentShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    // Creates executables in the graphics card
    glLinkProgram(shaderProgram);
    // Throw error if linker fails
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        printf("Error linking program: '%s'\n", errorLog);
        return;
    }

    // Throw error if shader program is not valid
    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        printf("Error validating program: '%s'\n", errorLog);
        return;
    }
}

int main() {

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
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "processamento-grafico", NULL, NULL);
	if (!mainWindow) { // throws error if failed
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information from the created window
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set the context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;


	if (glewInit() != GLEW_OK) {
		printf("GLEW initialization failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Creates a VAO holding triangle data
	createTriangle();
	// Creates and sets the shader program id to a global variable
	compileShaders();

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {
		// Get and handle user input events
		glfwPollEvents();

		// Clear the window
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Clear only the color (there is more data that can be cleared --like depth)
		glClear(GL_COLOR_BUFFER_BIT);

        // Set current shader program
        glUseProgram(shaderProgram);
        // Set current vertex array
        glBindVertexArray(VAO);

        // (what we want to draw, array offset, array stride --#vertices to read)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Unassign vertex array;
        glBindVertexArray(0);
        // Unassign shader after using it
        glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}