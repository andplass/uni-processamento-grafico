#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Set window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint shaderProgram;;
GLuint VAO_ROOF, VBO_ROOF;
GLuint VAO_FLOOR, VBO_FLOOR;
GLuint VAO_LEFT_WALL, VBO_LEFT_WALL;
GLuint VAO_RIGHT_WALL, VBO_RIGHT_WALL;
GLuint VAO_DOOR, VBO_DOOR;
GLuint VAO_WINDOW, VBO_WINDOW;
GLuint VAO_WINDOW_HORIZONTAL, VBO_WINDOW_HORIZONTAL;
GLuint VAO_WINDOW_VERTICAL, VBO_WINDOW_VERTICAL;

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


void createWindowVertical() {
	GLfloat vertices[] = {
		-0.3f, -0.3f, 0.0f, 0.0f, 0.0f,
		-0.3f, -0.55f, 0.0f, 0.0f, 0.0f
	};

	// Create VAO and set it to current buffer context
	glGenVertexArrays(1, &VAO_WINDOW_VERTICAL);
	glBindVertexArray(VAO_WINDOW_VERTICAL);

	// Create VBO
	glGenBuffers(1, &VBO_WINDOW_VERTICAL);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_WINDOW_VERTICAL);
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

void createWindowHorizontal() {
	GLfloat vertices[] = {
		-0.4f, -0.425f, 0.0f, 0.0f, 0.0f,
		-0.2f, -0.425f, 0.0f, 0.0f, 0.0f
	};

	// Create VAO and set it to current buffer context
	glGenVertexArrays(1, &VAO_WINDOW_HORIZONTAL);
	glBindVertexArray(VAO_WINDOW_HORIZONTAL);

	// Create VBO
	glGenBuffers(1, &VBO_WINDOW_HORIZONTAL);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_WINDOW_HORIZONTAL);
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

void createWindow() {
	GLfloat vertices[] = {
		-0.4f, -0.3f, 1.0f, 1.0f, 0.0f,
		-0.4f, -0.55f, 1.0f, 1.0f, 0.0f,
		-0.2f, -0.55f, 1.0f, 1.0f, 0.0f,
		-0.2f, -0.3f, 1.0f, 1.0f, 0.0f
	};

	// Create VAO and set it to current buffer context
	glGenVertexArrays(1, &VAO_WINDOW);
	glBindVertexArray(VAO_WINDOW);

	// Create VBO
	glGenBuffers(1, &VBO_WINDOW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_WINDOW);
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

void createDoor() {
	GLfloat vertices[] = {
		-0.1f, -0.9f, 0.5f, 0.2f, 0.1f,
		-0.1f, -0.5f, 0.5f, 0.2f, 0.1f,
		0.1f, -0.5f, 0.5f, 0.2f, 0.1f,
		0.1f, -0.9f, 0.5f, 0.2f, 0.1f
	};

	// Create VAO and set it to current buffer context
	glGenVertexArrays(1, &VAO_DOOR);
	glBindVertexArray(VAO_DOOR);

	// Create VBO
	glGenBuffers(1, &VBO_DOOR);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_DOOR);
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

void createLeftWall() {
	GLfloat vertices[] = {
		-0.5f, -0.9f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.0f, 0.0f, 0.0f, 0.0f
	};

	// Create VAO and set it to current buffer context
	glGenVertexArrays(1, &VAO_LEFT_WALL);
	glBindVertexArray(VAO_LEFT_WALL);

	// Create VBO
	glGenBuffers(1, &VBO_LEFT_WALL);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_LEFT_WALL);
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

void createRightWall() {
	GLfloat vertices[] = {
		0.5f, -0.9f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.0f, 0.0f
	};

	// Create VAO and set it to current buffer context
	glGenVertexArrays(1, &VAO_RIGHT_WALL);
	glBindVertexArray(VAO_RIGHT_WALL);

	// Create VBO
	glGenBuffers(1, &VBO_RIGHT_WALL);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_RIGHT_WALL);
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

void createFloor() {
	GLfloat vertices[] = {
		// floor
		-1.0f, -0.9f, 1.0f, 0.2f, 0.0f,
		1.0f, -0.9f, 1.0f, 0.2f, 0.0f
	};

	// Create VAO and set it to current buffer context
	glGenVertexArrays(1, &VAO_FLOOR);
	glBindVertexArray(VAO_FLOOR);

	// Create VBO
	glGenBuffers(1, &VBO_FLOOR);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_FLOOR);
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

void createRoof() {
    GLfloat vertices[] = {
        -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.75f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    };

    // Create VAO and set it to current buffer context
    glGenVertexArrays(1, &VAO_ROOF);
    glBindVertexArray(VAO_ROOF);

    // Create VBO
    glGenBuffers(1, &VBO_ROOF);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ROOF);
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

	// Create VAOs holding data
	createRoof();
	createFloor();
	createLeftWall();
	createRightWall();
	createDoor();
	createWindow();
	createWindowHorizontal();
	createWindowVertical();

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

		// Draw roof
		glBindVertexArray(VAO_ROOF);
        glDrawArrays(GL_TRIANGLES, 0, 3); // (what we want to draw, array offset, array stride --#vertices to read

		// Draw floor
		glBindVertexArray(VAO_FLOOR);
		glDrawArrays(GL_LINES, 0, 2);

		// Draw left wall
		glBindVertexArray(VAO_LEFT_WALL);
		glDrawArrays(GL_LINES, 0, 2);

		// Draw right wall
		glBindVertexArray(VAO_RIGHT_WALL);
		glDrawArrays(GL_LINES, 0, 2);

		// Draw door
		glBindVertexArray(VAO_DOOR);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		// Draw window
		glBindVertexArray(VAO_WINDOW);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		// Draw window horizontal
		glBindVertexArray(VAO_WINDOW_HORIZONTAL);
		glDrawArrays(GL_LINES, 0, 2);

		// Draw window vertical
		glBindVertexArray(VAO_WINDOW_VERTICAL);
		glDrawArrays(GL_LINES, 0, 2);

        // Unassign vertex array;
        glBindVertexArray(0);
        // Unassign shader after using it
        glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}