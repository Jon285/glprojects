#include "include/shader.h"

Shader::Shader(const char* vertexpath, const char* fragpath)
{
	std::string vertexCode;
	std::string fragCode;
	std::ifstream vFile;
	std::ifstream fFile;

	vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try 
	{
		vFile.open(vertexpath);
		fFile.open(fragpath);
		std::stringstream vStream, fStream;

		vStream << vFile.rdbuf();
		fStream << fFile.rdbuf();

		vFile.close();
		fFile.close();

		vertexCode = vStream.str();
		fragCode = fStream.str();
	}
	catch(std::ifstream::failure e)
	{
		std::cerr << "ERROR READING SHADER FILES" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragCode.c_str();

	unsigned int vertex, frag;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cerr << "ERROR DURING VERTEX SHADER COMPILATION: " << infoLog << std::endl;
	}

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fShaderCode, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		std::cerr << "ERROR DURING FRAGMENT SHADER COMPILATION: " << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, frag);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cerr << "ERROR WHILE LINKING SHADER PROGRAM: " << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(frag);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setbool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Shader::setint(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setfloat(const std::string &name, float value) const 
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
