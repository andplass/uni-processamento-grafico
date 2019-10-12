/* Hello Triangle - código fornecido em https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Computação Gráfica - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 7/4/2017
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLEW
//#define GLEW_STATIC //-- se habilitar, não precisa da dll
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Nossa classezinha que lê o arquivo de shader e o compila na OpenGL
// Exemplo retirado de https://learnopengl.com/#!Getting-started/Shaders
#include "Shader.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
GLuint VAO_TRIANGULO_1, VBO_TRIANGULO_1;


void createTriangle1() {
	GLfloat vertices[] = {
		// Positions          // Colors           
		537.5f,  300.0f, 0.0f,   1.0f, 0.0f, 0.0f,
		802.25f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
		1067.5f, 300.0f, 0.0f,   1.0f, 0.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO_TRIANGULO_1);
	glGenBuffers(1, &VBO_TRIANGULO_1);


	glBindVertexArray(VAO_TRIANGULO_1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_TRIANGULO_1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Orthographic Camera", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	/* get version info */
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Build and compile our shader program
	Shader ourShader("../shaders/transformations.vs", "../shaders/transformations.frag");

	// Setup buffers
	createTriangle1();

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		//aqui recupera o tamanho atual da janela, para correção do aspect ratio mais tarde
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Activate shader
		ourShader.Use();

		// Create transformations -- a first "camera" -- Orthographic Camera
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 ortho;
		
		//model = glm::rotate(model, (GLfloat)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
				
		//ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	
		//corrigindo o aspecto
		float ratio;
		float xMin = 0.0, xMax = 800.0, yMin = 600.0, yMax = 0.0;
		float zNear = 0.1, zFar = 1.0;
		if (width >= height)
		{
			ratio = width / (float)height;
			ortho =  glm::ortho(xMin*ratio, xMax*ratio, yMin, yMax,zNear,zFar);
		}
		else
		{
			ratio = height / (float)width;
			ortho =  glm::ortho(xMin, xMax, yMin*ratio, yMax*ratio, zNear, zFar);
		}

		// Get their uniform location
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
		// Pass them to the shaders
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// Note: currently we set the projection matrix each frame, but since the projection
		// matrix rarely changes it's often best practice to set it outside the main loop only once.
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho));

		// Draw
		glBindVertexArray(VAO_TRIANGULO_1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO_TRIANGULO_1);
	glDeleteBuffers(1, &VBO_TRIANGULO_1);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

