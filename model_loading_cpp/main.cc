#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/shader.h"
#include "include/camera.h"
#include "include/stb_image.h"
#include "include/model.h"

//handy functions
void handleInput(GLFWwindow*);
void mouseCallback(GLFWwindow*, double, double);
void scrollCallback(GLFWwindow* win, double xoffset, double yoffset);
void resizeCallback(GLFWwindow* window, int wid, int height);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	//setting everything up
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(800, 600, "MODEL LOADING", NULL, NULL);
	if(window == NULL)
	{
		std::cerr << "Failed to create window" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Error starting GLEW" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetFramebufferSizeCallback(window, resizeCallback);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE);
	
	Shader nanoShader("shaders/nanoVS.glsl", "shaders/nanoFS.glsl");
	Model nanoModel("nanosuit/nanosuit.obj");

	//just some fancy stuff
	std::cout << "Shaders compiled." << std::endl;
	std::cout << "Buffers Created." << std::endl;
	std::cout << "Rendering started.\n" << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

	while(!glfwWindowShouldClose(window))
	{
		//frame update
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		handleInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		nanoShader.use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		nanoShader.setmat4("projection", projection);
		nanoShader.setmat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		nanoShader.setmat4("model", model);
		nanoModel.Draw(nanoShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	//finish
	glfwTerminate();
	return EXIT_SUCCESS;
}

void handleInput(GLFWwindow* win)
{
	float speed = camera.MovementSpeed * deltaTime;
	if(glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);

	if(glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if(glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if(glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if(glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if(glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.Position -= camera.Up * speed;
	if(glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.Position += camera.Up * speed;
	if(glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		firstMouse = true;
}

void scrollCallback(GLFWwindow* win, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void mouseCallback(GLFWwindow* win, double xpos, double ypos)
{
	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void resizeCallback(GLFWwindow* window, int wid, int height)
{
	glViewport(0, 0, wid, height);
}
