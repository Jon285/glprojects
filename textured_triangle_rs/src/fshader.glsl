#version 330 core

in vec2 v_texture_pos;
out vec4 color;

uniform sampler2D tex;

void main()
{
	color = texture(tex, v_texture_pos);
}
