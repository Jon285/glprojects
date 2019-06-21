#ifndef SHADER_HEADER
#define SHADER_HEADER

#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

class Shader {
	public:
		unsigned int ID;

		Shader(const GLchar* vertex, const GLchar* frag);
		
		void use();
		void setbool(const std::string &name, bool value) const;
		void setint(const std::string &name, int value) const;
		void setfloat(const std::string &name, float value) const;
		void setvec2(const std::string &name,  glm::vec2 &value) const;
		void setvec3(const std::string &name, glm::vec3 &value) const;
		void setvec4(const std::string &name, glm::vec4 &value) const;
		void setmat2(const std::string &name, glm::mat2 &value) const;
		void setmat3(const std::string &name, glm::mat3 &value) const;
		void setmat4(const std::string &name, glm::mat4 &value) const;
};
#endif
