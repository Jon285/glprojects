#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/shader.h"
#include "include/stb_image.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void handleInput(GLFWwindow*);
void mouseCallback(GLFWwindow*, double, double);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(800, 600, "textures m8", NULL, NULL);
	if(window == NULL)
	{
		std::cerr << "Failed to create window" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	//glfwSetFramebufferSizeCallback(window, callback);

	glewExperimental = true;
	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Error starting GLEW" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	Shader my_shader("shaders/vShader.glsl", "shaders/fShader.glsl");

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_MULTISAMPLE);

	float vertices[] = {
    		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};	

	glm::vec3 cubePositions[] = {
  		glm::vec3( 0.0f,  0.0f,  0.0f), 
  		glm::vec3( 2.0f,  5.0f, -15.0f), 
  		glm::vec3(-1.5f, -2.2f, -2.5f),  
 	 	glm::vec3(-3.8f, -2.0f, -12.3f),  
  		glm::vec3( 2.4f, -0.4f, -3.5f),  
  		glm::vec3(-1.7f,  3.0f, -7.5f),  
  		glm::vec3( 1.3f, -2.0f, -2.5f),  
  		glm::vec3( 1.5f,  2.0f, -2.5f), 
  		glm::vec3( 1.5f,  0.2f, -1.5f), 
  		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};

	//Buffers and VAO configuration
	unsigned int VBO, VAO;
    	glGenVertexArrays(1, &VAO);
    	glGenBuffers(1, &VBO);

    	glBindVertexArray(VAO);

    	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    	glEnableVertexAttribArray(0);
    	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    	glEnableVertexAttribArray(1);

	std::cout << "Buffers Created." << std::endl;
	
	//load the textures
	unsigned int textID;
	glGenTextures(1, &textID);
	glBindTexture(GL_TEXTURE_2D, textID);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int height, wid, nrchannels;
	
	unsigned char *data = stbi_load("texture/ironb.png", &wid, &height, &nrchannels, 0);
	std::cout << "Texture file readed." << std::endl;
	if(data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wid, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	
	std::cout << "Textures loaded." << std::endl;

	my_shader.use();
	my_shader.setint("texture1", 0);

	GLint model_loc = glGetUniformLocation(my_shader.ID, "model");
	GLint proj_loc = glGetUniformLocation(my_shader.ID, "projection");
	GLint view_loc = glGetUniformLocation(my_shader.ID, "view");

	std::cout << "Rendering started.\n" << std::endl;
	
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	
	glm::mat4 proj;
	proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	while(!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		handleInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textID);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		my_shader.use();
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO);
		for(unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return EXIT_SUCCESS;
}

void handleInput(GLFWwindow* win)
{
	if(glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);

	float cameraSpeed = 2.5f * deltaTime;
	if(glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if(glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if(glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if(glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	cameraPos.y = 0.0f;
}

void mouseCallback(GLFWwindow* win, double xpos, double ypos)
{
	static float pitch = 0.0f;
	static float yaw = -90.0f;
	static bool firstMouse = true;
	static float lastX = 400, lastY = 300;

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

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw = glm::mod(yaw + xoffset, 360.0f);
	pitch += yoffset;

	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 frontCam;
	frontCam.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	frontCam.y = std::sin(glm::radians(pitch));
	frontCam.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	cameraFront = glm::normalize(frontCam);
}
