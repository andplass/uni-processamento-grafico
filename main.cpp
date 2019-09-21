#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Set window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shaderProgram;

const GLchar* vertexShader = "#version 410\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";
const GLchar* fragmentShader = "#version 410\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";


void createTriangle() {
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
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
    // Selects location = 0 shaderProgram
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    // Enable usage of vertex with location = 0
    glEnableVertexAttribArray(0);

    // Selects location = 1 shaderProgram
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    // Enable usage of vertex with location = 0
    glEnableVertexAttribArray(1);

    // Unbind buffer from context
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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