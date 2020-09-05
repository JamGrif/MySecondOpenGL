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

//Window Dimensions
const GLint Width = 600;
const GLint Height = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

//Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

//Camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = Width / 2.0f;
GLfloat lastY = Height / 2.0f;

bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

float x = 0.0;
float y = 0.0;
GLuint XoffsetLoc;
GLuint YoffsetLoc;

void init()
{
	//Set window settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Required for MacOS
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Stop resize of window

}

void postinit(GLFWwindow* window)
{
	//Set icon
	GLFWimage icons[1];
	icons[0].pixels = SOIL_load_image("res/textures/cowboy.jpg", &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
	glfwSetWindowIcon(window, 1, icons);
	SOIL_free_image_data(icons[0].pixels);

	//Gets actual size of screen, relative to density of screen
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	//Make the associated OPenGL context current for the window
	glfwMakeContextCurrent(window);

	//Remove mouse cursor 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_BLEND); //Help in blending image in the shape
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Enabled alpha support

	//Define viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//glEnable(GL_DEPTH_TEST);

	//Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
}

void display(GLFWwindow* window, double CurrentTime)
{
	
}

int main()
{
	//Init GLFW
	glfwInit();

	init();

	//Create a GLFW window that can use GLFW functions
	GLFWwindow* window = glfwCreateWindow(Width, Height, "Jamie Griffiths", nullptr, nullptr);

	postinit(window);
	
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


	GLuint vao;

	
	Shader shader("res/shaders/core.vert", "res/shaders/core.frag");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(1);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//Poll for and process events
		glfwPollEvents();
		DoMovement();

		//Clear display for next render
		glClear(GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use(); //Loads this shader onto the GPU
		XoffsetLoc = glGetUniformLocation(shader.Program, "Xoffset");
		glProgramUniform1f(shader.Program, XoffsetLoc, x);

		YoffsetLoc = glGetUniformLocation(shader.Program, "Yoffset");
		glProgramUniform1f(shader.Program, YoffsetLoc, y);

		

		glDrawArrays(GL_TRIANGLES, 0, 3); //Initaties pipeline processing

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		y += 0.1;
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		y -= 0.1;
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		x -= 0.1;
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		x += 0.1;
	}
}

//Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	/*if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos; //Reversed as Y coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	//camera.ProcessMouseMovement(xOffset, yOffset);
	*/
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	//camera.ProcessMouseScroll(yOffset);
}
