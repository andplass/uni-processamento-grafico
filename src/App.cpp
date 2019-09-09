#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

//#define GLEW_STATIC -- se habilitar, não precisa da dll
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Function prototypes
void key_callback(GLFWwindow* window,int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// SHADER CODE --------------------------------------------------------------------------------------
// Here is the actual program the shader will execute

// VERTEX (POINT)
// the shader version is the first bit of information needed to compile it
// the layout informs where is location 0 for the shader, in this case, zero itself
// this shader expects to receive a vec3 object, and create a position object with a constant 4D value (1.0) 
const GLchar* vertexShaderSource = "#version 410\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

// FRAGMENT (COLOR, OPACITY)
// this shader expects a vec4 as a object, and output a vec4 object with a constant opacity value (1.0)
const GLchar* fragmentShaderSource = "#version 410\n"
"uniform vec4 inputColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = inputColor;\n"
"}\n\0";

// APPLICATION START --------------------------------------------------------------------------------------
int main() {
    // INITIALIZATON PROCEDURES --------------------------------------------------------------------------------------
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	// Make the context of our window the main context on the current thread (set the created window in the context)
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	// Callbacks like window resizing or user input that will be received in the game loop
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Define the GL viewport dimensions
	// Setting the viewport dimensions smaller than the window`s will make it render in only part of the window
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

    // SHADER COMPILATION --------------------------------------------------------------------------------------
	// Here the shaders will be compiled. This means we will be informing the "shader creation engine"
	// what kind of object the buffer will be holding, which id the created shader should have,
	// what is the actual data the shader will be holding (our vertex positions), and the code the shader will be running
	// After compiling the shaders, their ids will be used to set them in the drawing context
	
	// VERTEX --------------------------------------------------------------------------------------
	// create an id for the vertex shader, creating a shader program of type vertex
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	// shaderSource will inform how we want to compile our shader 
	// vertexShader = created shader id
	// 1 = how many strings we are passing (strings with the shader`s source code, written at the top
	// &vertexShaderSource = the actual memory address for the source code
	// NULL = unimportant for now
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	
	// compileShader just compiles it
	glCompileShader(vertexShader);
	
	// Check for shader compile errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	// FRAGMENT SHADER COMPILE --------------------------------------------------------------------------------------
	// generate a GL_FRAGMENT_SHADER  and save its id
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// setup information about the shader 
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// compile it
	glCompileShader(fragmentShader);
	
	
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	
	// SHADER PROGRAM --------------------------------------------------------------------------------------
	// After creating the basic shaders necessary to draw, we link them together, creating a shader program,
	// which will have information both about how the vertexes should be positioned, and how they should be colored
	
	// create and get the program id
	GLuint shaderProgram = glCreateProgram();
	// attach to the program the vertex shader
	glAttachShader(shaderProgram, vertexShader);
	// attach to the program the fragment shader
	glAttachShader(shaderProgram, fragmentShader);
	// link them together
	glLinkProgram(shaderProgram);
	
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


    // VERTEX & COLOR DATA --------------------------------------------------------------------------------------
    // The vertex and fragment shaders now have been compiled into a program. All left to do is
    // inform the vertex position values and the color values to their shaders.
    
	// create the vertex position data
	// this value could be made dynamic by polling user input
	GLfloat vertices[] = {
	    // triangulo esquerdo
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 
		-0.5f,  0.0f, 0.0f,
        // triangulo direito
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 
		0.5f,  0.0f, 0.0f,  
	};
	
	// generate vertex array and vertex buffer objects
	GLuint VBO, VAO;
	// VAO = vertex array object
	glGenVertexArrays(1, &VAO);
	// VBO = vertex buffer object
	glGenBuffers(1, &VBO);
	
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);
 
 
    // bind the vbo to a GL_ARRAY_BUFFER type
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// copy our actual data (the vertices) to the previously bound vbo, and inform it how it what draw strategy should be used
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
    // SETTING VERTICES ATTRIBUTES
    // Here we tell opengl how we have defined a vertice in our array, how many vertices there are, and how they
    // should be read
    // *the first param specifies which vertex shader attribute we will use. we will default to 0, but setting it to other
    // numbers allows the same shader program to quickly change to previously set attributes without having to go through
    // all the work twice
    // *the second param specifies how many vertices it should start counting, starting from the byte offset set in the last param
    // *the third param specifies the type of data it will be receiving in the data (floats)
    // *the fourth param specifies if we want to normalize the data (unimportant for now, so we set it to false)
    // *the fifth param specifies the amount of the stride, or how many of the chosen data type should be "parsed" 
    // as one vertex. for now we know that our vertex holds three floats for x, y, z values, so we set it to 3
    // *the sixth param specifies an offset from where the buffer will be read. the first float in the array could be
    // ignored if set to 4
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	// vertex attribute function is disabled by default
	// we activate it by calling this function and passing in the attribute configuration of id 0 (the first param in the last function)
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	//Enviando a cor desejada (vec4) para o fragment shader
	GLint colorLoc = glGetUniformLocation(shaderProgram, "inputColor");
	assert(colorLoc > -1);
	glUseProgram(shaderProgram);
	glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);

	// Game loop
	// the WindowShouldClose function awaits for a window event to happen
	// (in this case the window close event) and leaves the game loop when it happens
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// RENDERING (finally) --------------------------------------------------------------------------------------
		// clear the next color buffer, setting the next background image to be rendered with the specified color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		
		// inform GL what kind od clear we are making (GL_COLOR_BUFFER_BIT, GL_STENCIL_BUFFER_BIT, ...)
		glClear(GL_COLOR_BUFFER_BIT);

		// use the created program
		glUseProgram(shaderProgram);
		// bind the vertex array to this loop
		// (multiple vertex configurations can be declared before hand and swapped at this point)
		// (swap between a triangle or a square layout of vertexes)
		glBindVertexArray(VAO);
		// actually draw something using the vertices and colors created
		// the first param specifies what type of opengl primitive will be used
		// the second param specifies from which vertex in the array we want to start reading from
		// the third param specifies until which vertex in the array we want to read
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// 
		glBindVertexArray(0);

		// Swap the screen buffers
		// The front buffer (active now) shows the image on the screen
		// The back buffer draws the next image while the front shows the image
		// Here the buffers are swapped, so all data manipulated in this loop
		// will be shown on screen while the next drawing loop runs
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW before stopping the program
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

