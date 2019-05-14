#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

GLuint loadShaders(const char* vertex, const char* fragment)
{
	//varievais que carregarao o ID dos shaders compilados
	GLuint vertexID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	
	//variaveis que carregam o codigo e o caminho pro arquivo do shader
	std::string vertexShader;
	std::ifstream vertexFile(vertex, std::ios::in);

	//checa se o arquivo foi aberto
	//se sim, faz as operacoes necessarias pra salvar o conteudo na string
	//do contrario reporta um erro e retorna
	if(vertexFile.is_open())
	{
		std::stringstream sstr;
		sstr << vertexFile.rdbuf();
		vertexShader = sstr.str();
		vertexFile.close();
	} else {
		std::cerr << "FAILED TO OPEN VERTEX SHADER FILE" << std::endl;
		return 0;
	}

	//basicamente o mesmo do bloco anterior, mas para o segundo shader
	std::string fragmentShader;
	std::ifstream fragmentFile(fragment, std::ios::in);

	if(fragmentFile.is_open())
	{
		std::stringstream sstr;
		sstr << fragmentFile.rdbuf();
		fragmentShader = sstr.str();
		fragmentFile.close();
	}

	//utilidades
	GLint resultado = GL_FALSE;
	int reportLength;

	//inicia a compilacao do shader de vertices
	std::cout << "Compilando shader: " << vertex << std::endl;
	const char* vertexPtr = vertexShader.c_str();
	glShaderSource(vertexID, 1, &vertexPtr, NULL);
	glCompileShader(vertexID);

	//checa o status da compilacao
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &resultado);
	glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &reportLength);
	if(reportLength > 0)
	{
		char* log[reportLength+1];
		glGetShaderInfoLog(vertexID, reportLength, NULL, log[0]);
		std::cout << "Vertex Compile Log: " << log << std::endl;
	}

	//compilacao do shader de fragmentos
	std::cout << "Compilando shader: " << fragment << std::endl;
	const char* fragmentPtr = fragmentShader.c_str();
	glShaderSource(fragmentID, 1, &fragmentPtr, NULL);
	glCompileShader(fragmentID);

	//checa o status da compilacao
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &resultado);
	glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &reportLength);
	if(reportLength > 0)
	{
		char* log[reportLength+1];
		glGetShaderInfoLog(fragmentID, reportLength, NULL, log[0]);
		std::cout << "Fragment Compile Log: " << log << std::endl;
	}

	//finalmente, cria o programa de shader
	std::cout << "Linkando programa." << std::endl;
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	glLinkProgram(programID);

	//checa o status do programa
	glGetProgramiv(programID, GL_LINK_STATUS, &resultado);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &reportLength);
	if(reportLength > 0)
	{
		char* prLog[reportLength+1];
		glGetProgramInfoLog(programID, reportLength, NULL, prLog[0]);
		std::cout << "Program Link Log: " << prLog << std::endl;
	}

	//faz a limpeza final e finaliza a funcao
	glDetachShader(programID, vertexID);
	glDetachShader(programID, fragmentID);

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);

	return programID;

}
