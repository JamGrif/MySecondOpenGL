//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//SOIL
#include "SOIL2.h"

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Shader.h"

GLint SCREENWIDTH;
GLint SCREENHEIGHT;

GLuint RenderingProgram;
GLuint vbo[2]; //VBO - Vertex Buffer Object - A buffer which stores EACH objects vertices 
GLuint vao[1]; //VAO - Vertex Array Object - Organizes buffers

glm::vec3 camera;
glm::vec3 cubeLoc;

glm::mat4 pMat = glm::mat4(1.0f); //Proj matrix, view matrix, model matrix, m+v matrix
glm::mat4 mvMat = glm::mat4(1.0f);
glm::mat4 vMat = glm::mat4(1.0f);
glm::mat4 mMat = glm::mat4(1.0f);
void setupVertices()
{
	float vPositions[108] =
	{
			-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};

	glGenVertexArrays(1, vao); //Generate VAO and returns int ID stored in "vao"
	glBindVertexArray(vao[0]); //Makes specified vao active

	//Buffers need a corresponding vertex attribute in vertex shader
	glGenBuffers(2, vbo); //Generate VBO and returns int ID stored in "vbo"
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); //Makes specificed vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions), vPositions, GL_STATIC_DRAW); //Copies array into the active buffer
}

void init(GLFWwindow* window)
{
	//Set icon
	GLFWimage icons[1];
	icons[0].pixels = SOIL_load_image("res/textures/cowboy.jpg", &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
	glfwSetWindowIcon(window, 1, icons);
	SOIL_free_image_data(icons[0].pixels);

	//Remove mouse cursor 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwGetFramebufferSize(window, &SCREENWIDTH, &SCREENHEIGHT);

	//Define viewport dimensions
	glViewport(0, 0, SCREENWIDTH, SCREENHEIGHT);

	//Build perspective matrix
	float aspect = (float)SCREENWIDTH / (float)SCREENHEIGHT;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); //1.0472 is 60 degrees

	RenderingProgram = Shader::CreateShader("res/shaders/core.vert", "res/shaders/core.frag");

	camera.x = 0.0f; camera.y = 0.0f; camera.z = 8.0f;
	cubeLoc.x = 0.0f; cubeLoc.y = -2.0f; cubeLoc.z = 0.0f;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	setupVertices();
}



void display(GLFWwindow* window, double CurrentTime)
{
	//Clear display for next render
	glClear(GL_DEPTH_BUFFER_BIT);

	//shader.Use(); //Loads this shader onto the GPU
	glUseProgram(RenderingProgram);

	//Get uniform variable location for MV and proj matrix
	GLuint mvLoc = glGetUniformLocation(RenderingProgram, "mv_matrix");
	GLuint projLoc = glGetUniformLocation(RenderingProgram, "proj_matrix");

	//Build view, model and model-view matrix
	vMat = glm::translate(glm::mat4(1.0), glm::vec3(-camera.x, -camera.y, -camera.z));
	mMat = glm::translate(glm::mat4(1.0), glm::vec3(cubeLoc.x, cubeLoc.y, cubeLoc.z));
	mvMat = vMat * mMat;

	//Copy matrix's to uniform variables in shader
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); //Makes specificed vbo active
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0); //Associate active buffer with a vertex attribute in shader. All values sent through a buffer to a vertex attribute will be interpolated further down pipeline
	glEnableVertexAttribArray(0); //Enable the vertex attribute

	glDrawArrays(GL_TRIANGLES, 0, 36); //Initaties pipeline processing
}

int main()
{
	//Init GLFW
	glfwInit();

	//Set window settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Version 4.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);		     //6
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Required for MacOS
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Stop resize of window

	//Create a GLFW window that can use GLFW functions
	GLFWwindow* window = glfwCreateWindow(600, 600, "Jamie Griffiths", nullptr, nullptr);

	//Make the associated OpenGL context current for the window
	glfwMakeContextCurrent(window);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	//GLEW use a modern approach allowing retrieve function pointers and extensions
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "Failure to initialize GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	init(window);

	while (!glfwWindowShouldClose(window))
	{
		//Poll for and process events
		glfwPollEvents();

		display(window, glfwGetTime());

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}


