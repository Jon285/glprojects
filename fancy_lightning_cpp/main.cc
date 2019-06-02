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

//handy functions
void handleInput(GLFWwindow*);
void mouseCallback(GLFWwindow*, double, double);
void scrollCallback(GLFWwindow* win, double xoffset, double yoffset);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	//setting everything up
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(800, 600, "lightning wooo", NULL, NULL);
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

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_MULTISAMPLE);

	Shader cubeShader("shaders/cubeVS.glsl", "shaders/cubeFS.glsl");
	Shader lightShader("shaders/lightVS.glsl", "shaders/lightFS.glsl");
	std::cout << "Shaders compiled." << std::endl;

	//a cube has more vertices than you think
	float vertices[] = {
    		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
   		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
  	 	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
  		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
  		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	//Buffers and VAO configuration
	unsigned int VBO, cubeVAO, lightVAO;
    	
	//one VAO for each cube
	glGenVertexArrays(1, &cubeVAO);
	glGenVertexArrays(1, &lightVAO);
    	
	//since we'll have two cubes they'll share the same vertex buffer
	glGenBuffers(1, &VBO);

	//just normal VAO setup, nothing to see here
    	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    	
	glBindVertexArray(cubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//just some fancy stuff
	std::cout << "Buffers Created." << std::endl;
	std::cout << "Rendering started.\n" << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

	//the light color
	glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);

	//the cube light parameters
	glm::vec3 cube_ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 cube_diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 cube_specular = glm::vec3(0.5f, 0.5f, 0.5f);

	//the light parameters
	glm::vec3 light_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 light_diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 light_specular = glm::vec3(1.0f, 1.0f, 1.0f);

	while(!glfwWindowShouldClose(window))
	{
		//frame update
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		handleInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//change the light color on each frame
		light_color.x = std::cos(glfwGetTime() * 2.0f);
		light_color.y = std::sin(glfwGetTime() * 0.7f);
		light_color.z = std::cos(glfwGetTime() * 1.5f);

		//make the light spin around the cube
		lightPos.x = std::sin(glfwGetTime()) * 2.0f;
		lightPos.z = std::cos(glfwGetTime()) * 2.0f;

		cubeShader.use();
		cubeShader.setvec3("viewPos", camera.Position);

		cube_ambient = light_color * glm::vec3(0.5f);
		cube_diffuse = cube_ambient * glm::vec3(0.2f);

		//send the material thingies to the shader
		cubeShader.setvec3("material.ambient", cube_ambient);
		cubeShader.setvec3("material.diffuse", cube_diffuse);
		cubeShader.setvec3("material.specular", cube_specular);
		cubeShader.setfloat("material.shininess", 124.0f);

		//same with the light
		cubeShader.setvec3("light.ambient", light_ambient);
		cubeShader.setvec3("light.diffuse", light_diffuse);
		cubeShader.setvec3("light.specular", light_specular);
		cubeShader.setvec3("light.position", lightPos);

		//some math stuff to set the stage
		glm::mat4 proj;
		proj = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

		glm::mat4 view = camera.GetViewMatrix();
		cubeShader.setmat4("projection", proj);
		cubeShader.setmat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		cubeShader.setmat4("model", model);

		//finally draw the cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//do all the same with the light cube
		lightShader.use();
		lightShader.setmat4("projection", proj);
		lightShader.setmat4("view", view);
		
		model = glm::mat4(1.0f);
		
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.setmat4("model", model);

		lightShader.setvec3("Color", light_color);

		//and draw!
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//clean everything up
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

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
