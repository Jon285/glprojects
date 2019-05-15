#version 330 core

in vec2 pos;
in vec2 texture_pos;
out vec2 v_texture_pos;
uniform mat4 matrix;

void main()
{
	v_texture_pos = texture_pos;
	gl_Position = matrix * vec4(pos, 0.0, 1.0);
}
