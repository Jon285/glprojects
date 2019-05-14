//OpenGL headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//custom headers
#include "shaderloader/loader.hpp"
#include "math/mat/matrix4.hpp"

//C and C++ headers
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cassert>

int main()
{
	//start glfw
	if(!glfwInit())
	{
		std::cerr << "Falhou em iniciar GLFW!" << std::endl;
		return EXIT_FAILURE;
	}

	//set OpenGL option and create window with context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Teste", NULL, NULL);

	if(window == nullptr)
	{
		std::cerr << "Falhou em criar janela!" << std::endl;
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glewExperimental = true;
	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Falhou em iniciar GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	//color used on the background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	//the pyramid
	GLfloat vertice[] = {
		-1.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f};

	//vertex buffer created 
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertice), vertice, GL_STATIC_DRAW);

	unsigned int indice[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2};

	//indice buffer created
	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice), indice, GL_STATIC_DRAW);

	//program compiled
	GLuint programa = loadShaders("pVS.glsl", "pFS.glsl");

	//get uniform from the shader and start the main loop
	GLuint gWorldLocation = glGetUniformLocation(programa, "gWorld"); 
	while(glfwWindowShouldClose(window) == 0)
	{
		//clear the screen and bind the shader program
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(programa);

	//	scale from which que piramid spins
		static float scale = 0.0f;
		scale += 0.02f;
		
		//spin the pyramid
		Matrix4f World;
		World.m[0][0] = std::cos(scale); World.m[0][1] = 0.0f;  World.m[0][2] = -std::sin(scale);  World.m[0][3] = 0.0f;
		World.m[1][0] = 0.0f; 		 World.m[1][1] = 1.0f; 	World.m[1][2] = 0.0f;  		   World.m[1][3] = 0.0f;
		World.m[2][0] = std::sin(scale); World.m[2][1] = 0.0f;  World.m[2][2] = std::cos(scale);   World.m[2][3] = 0.0f;
		World.m[3][0] = 0.0f;		 World.m[3][1] = 0.0f;  World.m[3][2] = 0.0f;  		   World.m[3][3] = 1.0f;

		//send data to the shader
		glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);

		//send data to buffers
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		//draw
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);

		//self explanatory 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	//clear everything before exiting
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(programa);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();

	return EXIT_SUCCESS;
}
