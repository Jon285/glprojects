#ifndef SHADER_HEADER
#define SHADER_HEADER

#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
	public:
		unsigned int ID;

		Shader(const GLchar* vertex, const GLchar* frag);
		
		void use();
		void setbool(const std::string &name, bool value) const;
		void setint(const std::string &name, int value) const;
		void setfloat(const std::string &name, float value) const;
};
#endif
